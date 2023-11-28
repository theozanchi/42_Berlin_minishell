/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_executer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschott <jschott@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 17:54:35 by jschott           #+#    #+#             */
/*   Updated: 2023/11/28 11:14:45 by jschott          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief saves current stdin&stdout redirects to given file descriptors,  
 * executes and saves exit code of command and resets stdin & stout to initial 
 * states
 * @param fd_in_out array with file descriptors to read & write from/to
 * @param cmd builtin command to execute
 * @param data main data structure
*/
int	execute_builtin(int *fd_pipes, int pos, t_commands *cmd, t_data *data)
{
	int	original_fd[2];
	int	exit_code;

	original_fd[0] = dup(0);
	original_fd[1] = dup(1);
	dup2(fd_pipes[pos], 0);
	close_unused_fd(fd_pipes, pos, FDX_OW, (2 * cmd_count(data->commands)));
	dup2(fd_pipes[pos + 3], 1);
	exit_code = launch_builtin(cmd, data);
	close_fd(&fd_pipes[pos + 3]);
	dup2(original_fd[0], 0);
	dup2(original_fd[1], 1);
	return (exit_code);
}

/**
 * @brief redirects stdin&stdout to given file descriptors,  
 * executes and saves exit code of command
 * @param fd_in_out array with file descriptors to read & write from/to
 * @param cmd builtin command to execute
 * @param data main data structure
*/
int	execute_env(int *fd_pipes, int pos, t_commands *cmd, t_data *data)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (EXIT_FAILURE);
	if (pid == 0)
	{
		dup2(fd_pipes[pos], 0);
		dup2(fd_pipes[pos + 3], 1);
		close_unused_fd(fd_pipes, pos, FDX_RW, (2 * cmd_count(data->commands)));
		exit (command_executer(cmd, data));
	}
	close_fd(&fd_pipes[pos]);
	return (EXIT_SUCCESS);
}

int	catch_child_execs(pid_t *pid, int num, t_data *data, int *fd_pipes)
{
	int	i;
	int	exit_code;

	i = -1;
	while (++i < num)
	{
		if (pid[i] == 0)
		{
			waitpid(pid[i], &exit_code, 0);
			data->wstatus = WIFEXITED(exit_code);
			close_fd(&fd_pipes[(i * 2)]);
			close_fd(&fd_pipes[(i * 2) + 3]);
		}
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief executes multiple commands in a pipeline by forking in processes
 * @param fd_pipes array with file descriptors
 * @param pid array with process ids for executing childs
 * @param wpid array with process ids for waiting childs
 * @param data main data structure
 * @return exit code
*/
int	execute_pipeline(int *fd_pipes, pid_t *pid, t_data *data)
{
	t_commands	*cmd;
	int			i;

	if (!fd_pipes || !pid || !data)
		return (EXIT_FAILURE);
	i = 0;
	cmd = data->commands;
	while (cmd)
	{
		if (cmd_is_a_builtin(cmd))
		{
			execute_builtin (fd_pipes, i * 2, cmd, data);
			pid[i] = 1;
		}
		else
			execute_env (fd_pipes, i * 2, cmd, data);
		i++;
		cmd = cmd->next;
	}
	catch_child_execs(pid, i, data, fd_pipes);
	return (EXIT_SUCCESS);
}
