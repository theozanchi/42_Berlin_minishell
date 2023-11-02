/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschott <jschott@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 16:22:45 by jschott           #+#    #+#             */
/*   Updated: 2023/11/02 12:16:17 by jschott          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"
#include "minishell.h"

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

int	cmd_count(t_commands *cmds)
{
	int			i;
	t_commands	*ptr;

	i = 0;
	ptr = cmds;
	while (ptr)
	{
		ptr = ptr->next;
		i++;
	}
	return (i);
}

int	executer(t_output **out_redirect, t_commands *cmds, t_input **in_redirect, char **env)
{
	int		i;
	int		cmds_num;
	int		**fd_pipes;
	pid_t	pid;

	cmds_num = cmd_count(cmds);
	i = 0;
	fd_pipes = ft_calloc(cmds_num, 2 * sizeof(int));
	while (cmds->next)
	{
		pipe(fd_pipes[i]);
			// ERROR MGMT TBD
		pid = fork();
		if (pid == -1)
			return (0); // ERROR MGMT TBD
		if (pid == 0 && i > 0)
			pipe2pipe(fd_pipes[i - 1], cmds, fd_pipes[i], env);
		else if (pid == 0)
			fd2pipe(pid, cmds, in_redirect, env);
		if (pid != 0)
			wait (NULL);
		++i;
		cmds = cmds->next;
	}
	i = -1;
	while (out_redirect[++i])
		pipe2fd (out_redirect[i], cmds, fd_pipes[i], env);
	free (fd_pipes);
	return (1);
}
