/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschott <jschott@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 16:22:45 by jschott           #+#    #+#             */
/*   Updated: 2023/11/10 18:46:09 by jschott          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"

void	pipe2fd(t_output **out, t_commands *cmd, int *fd_pipe, char **env)
{
	int	i;
	int	fd_out;

	printf("\nHELLO P2FD\n%s\n",out[0]->path);
	// printf("executer check cmd:%s\n", cmd->command);
	i = -1;
	if (fd_pipe)
	{
		// printf("pipe found:\nin:  %i %i\nout: %s\n", fd_pipe[0], fd_pipe[1], out[0]->path);
		dup2(fd_pipe[0], 0);
		close(fd_pipe[1]);
	}
	if (!out[0]->path)
	{
		dup2(1, 1);
		// close(fd_pipe[1]);
		cmd_execute(cmd, env);
		exit (0);
	}
	while (out && out[++i])
	{
		fd_out = open(out[i]->path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (fd_out <= 0)
			return ; //ERROR MGMT TBD
		dup2(fd_out, 1);
		// close(fd_pipe[1]);
		cmd_execute(cmd, env);
		close (fd_pipe[0]);
		close (fd_out);
	}
	exit (0);
}

void	pipe2pipe(int *fd_pipeout, t_commands *cmd, int *fd_pipein, char **env)
{
	printf("\nHELLO P2P\n");
	printf("executer check cmd:%s\n", cmd->command);
	if (!fd_pipeout || !fd_pipein)
		return ; //ERROR MGMT TBD
	printf("pipe found:\nin:  %i %i\nout: %i %i\n", fd_pipein[0], fd_pipein[1], fd_pipeout[0], fd_pipeout[1]);
	// close(fd_pipein[1]);
	// close(fd_pipeout[0]);

	dup2(fd_pipein[0], 0);
	dup2(fd_pipeout[1], 1);
	
	cmd_execute(cmd, env);

	// close(fd_pipein[0]);
	// close(fd_pipeout[1]);
	exit (0);
}

void	fd2pipe(int *fd_pipe, t_commands *cmd, t_input **in, char **env)
{
	int	fd_in;

	printf("\nHELLO FD2P\n%s\n",in[0]->path);
	printf("executer check cmd:%s\n", cmd->command);
	if (in[0] != 0)
	{
		fd_in = open(in[0]->path, O_RDONLY);
		if (fd_in <= 0)
			return ;
		dup2(fd_in, 0);
	}
	printf("pipe found:\nout: %i %i\n", fd_pipe[0], fd_pipe[1]);
	dup2(fd_pipe[1], 1);
	close(fd_pipe[0]);
	cmd_execute(cmd, env);
	// close(fd_pipe[1]);
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

void printf_pipes(int *pipes)
{
	int	i = -1;

	printf("pipe fds:\n");
	while (pipes[++i] != '\0')
		printf("%i ", pipes[i]);
	printf("\n");
}

int	executer(t_output **out_redirect, t_commands *cmds, \
				t_input **in_redirect, char **env)
{
	int		i;
	int		cmds_num;
	int		*fd_pipes;
	int		*lastpipe;
	pid_t	*pid;

	printf("executer check cmd:%s\n", cmds->command);
	cmds_num = cmd_count(&cmds);
	lastpipe = 0;
	i = 0;
	fd_pipes = (int *) ft_calloc(cmds_num, 2 * sizeof(int));
	fd_pipes[cmds_num] = '\0';
	pid = (pid_t *) ft_calloc (cmds_num - 1, sizeof(pid_t));
	while (fd_pipes && pid && i < cmds_num - 1)
	{
		printf_pipes(&fd_pipes[i*2]);
		if (pipe(&fd_pipes[i * 2]) < -1)
			printf("ERROR\n"); // ERROR MGMT TBD
		pid[i] = fork();
		if (pid[i] == -1)
			return (0); // ERROR MGMT TBD
		if (pid[i] == 0 && i > 0)
			pipe2pipe(&fd_pipes[i * 2], cmds, &fd_pipes[(i * 2) - 2], env);
		else if (pid[i] == 0)
			fd2pipe(&fd_pipes[i * 2], cmds, in_redirect, env);
		if (pid[i] != 0)
		{
			wait (NULL);
			++i;
			cmds = cmds->next;
		}
	}
	if (fd_pipes && pid && out_redirect)
		pipe2fd (out_redirect, cmds, &fd_pipes[(i - 1) * 2], env);
	dup2(1, 1);
	dup2(0, 0);
	printf("\n+++\nDONE\n+++\n");
	free (fd_pipes);
	free (pid);
	return (1);
}
