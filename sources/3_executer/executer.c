/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschott <jschott@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 16:22:45 by jschott           #+#    #+#             */
/*   Updated: 2023/11/01 18:36:43 by jschott          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"

void	pipe2fd(t_output **out, t_commands *cmd, int *fd_pipe, char **env)
{
	int	i;
	int	fd_out;

	i = 0;
	if (fd_pipe)
	{
		dup2(fd_pipe[0], 0);
		close(fd_pipe[1]);
	}
	if (!out)
	{
		dup2(fd_out, 1);
		cmd_execute(cmd, env);
		close (fd_out);
	}
	while (out[++i])
	{
		fd_out = open(out[i], O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (fd_out <= 0)
			return ; //ERROR MGMT TBD
		dup2(fd_out, 1);
		cmd_execute(cmd, env);
		close (fd_out);
	}
}

void	pipe2pipe(int *fd_pipeout, t_commands *cmd, int *fd_pipein, char **env)
{
	if (!fd_pipeout || !fd_pipein)
		return ; //ERROR MGMT TBD
	dup2(fd_pipeout[1], 1);
	close(fd_pipeout[0]);
	dup2(fd_pipein[0], 0);
	close(fd_pipein[1]);
	cmd_execute(cmd, env);
}

void	fd2pipe(int *fd_pipe, t_commands *cmd, t_input **in, char **env)
{
	int	fd_in;

	if (in)
	{
		fd_in = open(in[0], O_RDONLY);
		if (fd_in <= 0)
			return ;
		dup2(fd_in, 0);
	}
	dup2(fd_pipe[1], 1);
	cmd_execute(cmd, env);
	close (fd_in);
}

int	executer(t_output **out_redirect, t_commands *cmds, t_input **in_redirect, char **env)
{
	int	i;
	int	fd_pipe_0[2];
	int	fd_pipe_1[2];
	int	pid;

	i = 0;
/* 
 * as many processes as commands 
 */
	if (cmds->next)
	{
		pipe(fd_pipe_0);
			// MISSING ERROR EXIT WHEN PIPING FAILS
		pid = fork();
		if (pid == -1)
			return (0); // ERROR MGMT TBD
		if (pid == 0)
			fd2pipe(pid, cmds, in_redirect, env);
		if (pid != 0)
		{
			cmds = cmds->next;
			while (cmds->next)
			{
				pipe(fd_pipe_1);
				pid = fork();
				if (pid == -1)
					return (0); // ERROR MGMT TBD
				if (pid == 0)
					pipe2pipe(fd_pipe_1, cmds, fd_pipe_0, env);
				if (pid != 0)
					wait (NULL)
					

			}
		}
	}
/* 
 * create and/or open all given output files 
 */
	i = -1;
	while (out_redirect[++i])
	{
		fd_out = open(out_redirect[i], O_WRONLY | O_CREAT | O_TRUNC, 0666);
		read_pipe (cmds, fd_out, fd_pipe, env);
	}
}


int	execute(int *fd_out, int *fd_in, t_commands cmd, char **env)
{
	if (fd_in <= 0)
		exit ; // ERROR MGMT TBD
	dup2(fd_in[0], 0);
	close(fd_in[1]);
	dup2(fd_out[1], 1);
	close(fd_out[0]);
	exec(cmd, env);
	close(fd_in[0]);
	return (1);
}
