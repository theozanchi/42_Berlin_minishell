/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschott <jschott@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 16:22:45 by jschott           #+#    #+#             */
/*   Updated: 2023/11/07 17:59:02 by jschott          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"

void	pipe2fd(t_output **out, t_commands *cmd, int *fd_pipe, char **env)
{
	int	i;
	int	fd_out;

	printf("HELLO P2FD\n");
	i = -1;
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
		fd_out = open(out[i]->path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (fd_out <= 0)
			return ; //ERROR MGMT TBD
		dup2(fd_out, 1);
		cmd_execute(cmd, env);
		close (fd_out);
	}
	exit (0);
}

void	pipe2pipe(int *fd_pipeout, t_commands *cmd, int *fd_pipein, char **env)
{
	printf("HELLO P2P\n");
	if (!fd_pipeout || !fd_pipein)
		return ; //ERROR MGMT TBD
	dup2(fd_pipeout[1], 1);
	close(fd_pipeout[0]);
	dup2(fd_pipein[0], 0);
	close(fd_pipein[1]);
	cmd_execute(cmd, env);
	exit (0);
}

void	fd2pipe(int *fd_pipe, t_commands *cmd, t_input **in, char **env)
{
	int	fd_in;

	printf ("%s\n", cmd->command);
	if (in[0])
	{
		fd_in = open(in[0]->path, O_RDONLY);
		printf("fd_in: %i", fd_in);
		if (fd_in <= 0)
			return ;
		dup2(fd_in, 0);
	}
	dup2(fd_pipe[1], 1);
	cmd_execute(cmd, env);
	close (fd_in);
	exit (0);
}

int	cmd_count(t_commands **cmds)
{
	int			i;
	t_commands	*ptr;

	i = 0;
	ptr = *cmds;
	while (ptr)
	{
		ptr = ptr->next;
		i++;
	}
	return (i);
}

int	executer(t_output **out_redirect, t_commands *cmds, \
				t_input **in_redirect, char **env)
{
	int		i;
	int		cmds_num;
	int		**fd_pipes;
	int		*lastpipe;
	pid_t	*pid;

	cmds_num = cmd_count(&cmds);
	lastpipe = 0;
	i = 0;
	fd_pipes = (int **) ft_calloc(cmds_num - 1, 2 * sizeof(int));
	pid = (pid_t *) ft_calloc (cmds_num - 1, sizeof(pid_t));
	while (fd_pipes && pid && i < cmds_num)
	{
		pipe(fd_pipes[i]);
			// ERROR MGMT TBD
		pid[i] = fork();
		if (pid[i] == -1)
			return (0); // ERROR MGMT TBD
		if (pid[i] == 0 && i > 0)
			pipe2pipe(fd_pipes[i], cmds, fd_pipes[i - 1], env);
		else if (pid[i] == 0)
			fd2pipe(fd_pipes[i], cmds, in_redirect, env);
		if (pid[i] != 0)
		{
			wait (NULL);
			++i;
			cmds = cmds->next;
		}
		else 
			exit (0);
	}
	lastpipe = fd_pipes[i - 1];
	i = -1;
	if (fd_pipes && pid && out_redirect)
		pipe2fd (out_redirect, cmds, lastpipe, env);
	printf("\n+++\nDONE\n+++\n");
	free (fd_pipes);
	free (pid);
	return (1);
}
