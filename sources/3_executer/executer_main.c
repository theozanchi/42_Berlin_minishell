/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschott <jschott@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 16:22:45 by jschott           #+#    #+#             */
/*   Updated: 2023/11/09 18:09:04 by jschott          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"

void	pipe2fd(t_output **out, t_commands *cmd, int *fd_pipe, char **env)
{
	int	i;
	int	fd_out;

	printf("\nHELLO P2FD\n%s\n",out[0]->path);
	printf("executer check cmd:%s\n", cmd->command);
	i = -1;
	if (fd_pipe)
	{
		printf("pipe exists\n");
		dup2(fd_pipe[0], 0);
		close(fd_pipe[1]);
	}
	printf("pipe not found\n");
	if (!out)
	{
		dup2(fd_out, 1);
		cmd_execute(cmd, env);
		close (fd_out);
	}
	while (out && out[++i])
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
	printf("\nHELLO P2P\n");
	printf("executer check cmd:%s\n", cmd->command);
	// if (!fd_pipeout || !fd_pipein)
		// return ; //ERROR MGMT TBD
	printf("pipe found\n");
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

	printf("\nHELLO FD2P\n%s\n",in[0]->path);
	printf("executer check cmd:%s\n\n", cmd->command);
	if (in[0])
	{
		fd_in = open(in[0]->path, O_RDONLY);
		if (fd_in <= 0)
			return ;
		dup2(fd_in, 0);
	}
	printf("pipe found\n");
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

	printf("executer check cmd:%s\n", cmds->command);
	cmds_num = cmd_count(&cmds);
	lastpipe = 0;
	i = 0;
	fd_pipes = (int **) ft_calloc(cmds_num - 1, 2 * sizeof(int));
	pid = (pid_t *) ft_calloc (cmds_num - 1, sizeof(pid_t));
	while (fd_pipes && pid && i < cmds_num - 1)
	{
		pipe(fd_pipes[i]);
			// ERROR MGMT TBD
		printf("pipes: %i | %i\n", fd_pipes[0][0]);//, fd_pipes[i+1][0], fd_pipes[i+1][1]);
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
	}
	// lastpipe = fd_pipes[i];
	if (fd_pipes && pid && out_redirect)
		pipe2fd (out_redirect, cmds, fd_pipes[i], env);
	dup2(1, 1);
	dup2(0, 0);
	printf("\n+++\nDONE\n+++\n");
	free (fd_pipes);
	free (pid);
	return (1);
}
