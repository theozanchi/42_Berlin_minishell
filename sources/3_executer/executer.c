/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschott <jschott@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 16:22:45 by jschott           #+#    #+#             */
/*   Updated: 2023/10/31 19:26:12 by jschott          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"

int	read_pipe(char *cmd, char *file_out, int *fd_pipe, char **env)
{
	int	fd_out;

	wait (NULL);
	fd_out = open(file_out, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	// if (fd_out <= 0)
	//	return (error_mgmt(OUTPUT_ERR, file_out));
	dup2(fd_out, 1);
	dup2(fd_pipe[0], 0);
	close(fd_pipe[1]);
	exec(cmd, env);
	close(fd_out);
	return (1);
}

int	executer(t_output **out_redirect, t_commands *cmds, t_input *in_redirect)
{
	int	fd_out;
	int	i;
	int	fd_pipe[2];
	int	pid;

	i = 0;
/* 
 * create and/or open all given input files 
 */
	while (in_redirect[i])
	{
		fd_in = open(out_redirect[i], O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (!out_redirect[++i])
			break ;
		close(fd_out);
	}
	close(fd_out);
/* 
 * as many processes as commands 
 */
	if (cmds->next)
	{
		pipe(fd_pipe);
			// MISSING ERROR EXIT WHEN PIPING FAILS
		pid = fork();
		if (pid == -1)
			exit(0); // PROPER EXIT TBD
		if (pid == 0)
			// CHILD_PROCESS
		if (pid != 0)
		{
			wait (NULL);
			executer(fd_pipe[1], cmds->next, in_redirect);
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
