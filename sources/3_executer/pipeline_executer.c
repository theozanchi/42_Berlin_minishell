/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_executer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschott <jschott@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 17:54:35 by jschott           #+#    #+#             */
/*   Updated: 2023/11/21 12:58:00 by jschott          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief closes a given file descriptor, that is neither stdin, stdout or stderr
 * @param fd file descriptor
 * @return 
 */
int	close_fd(int fd)
{
	if (fd > 2)
		return (close(fd));
	return (0);
}

/**
 * @brief closes all file desrciptor in an array
 * @param fd_pipes array with file descriptors
*/
void	close_all_fd(int *fd_pipes)
{
	int	i;

	i = -1;
	if (!fd_pipes)
		return ;
	while (fd_pipes[++i])
		close_fd (fd_pipes[i]);
}

/**
 * @brief redirects fds to given values and calls function that executes
 * @param fd_out desired filedescriptor for output
 * @param cmd command to execute incl. arguments and flags
 * @param fd_in desired filedescriptor for output
 * @param env environmental variable
 */
void	fd2fd(int *fd_pipes, t_commands *cmd, t_data *data)
{
	int	fd_in;
	int	fd_out;

	fd_in = fd_pipes[0];
	fd_out = fd_pipes[3];
	if (!cmd || !data)
		write(2, "FDF2FDF ERROR\n", 14);
	close_fd(fd_pipes[1]);
	close_fd(fd_pipes[2]);
	dup2(fd_out, 1);
	dup2(fd_in, 0);
}

/**
 * @brief Waits for a given childprocess of pid, saves its exec-code to data
 * 			and closes pipes, used by child
 * @param fd_out fd of the output pipe
 * @param fd_in fd of input pipe
 * @param pid process id of child to wait to exit
 * @param data  main datastructure
*/
void	parent(int *fd_pipes, pid_t pid, t_data *data, t_commands *cmd)
{
	int	status;
	int	orig_fdin;
	int	orig_fdout;
	int	fd_in;
	int	fd_out;

	waitpid(pid, &status, 0);
	fd_in = fd_pipes[0];
	fd_out = fd_pipes[3];
	// close_fd (fd_in);
	// close_fd (fd_out);
	if (cmd_is_a_builtin(cmd))
	{
		orig_fdout = dup(1);
		orig_fdin = dup(0);
		close_fd (fd_pipes[1]);
		close_fd (fd_pipes[2]);
		dup2(fd_out, 1);
		dup2(fd_in, 0);
		data->wstatus = launch_builtin(cmd, data);
		dup2(orig_fdout, 1);
		dup2(orig_fdin, 0);
	}
	else
		data->wstatus = WEXITSTATUS(status);
	close_fd (fd_in);
	close_fd (fd_out);
}

/**
 * @brief executes multiple commands in a pipeline by forking in processes
 * @param fd_pipes array with file descriptors
 * @param pid array with process ids
 * @param data main data structure
 * @return exit code
*/
int	execute_pipeline(int *fd_pipes, pid_t *pid, t_data *data)
{
	int			i;
	int			j;
	t_commands	*cmd;

	if (!fd_pipes || !pid || !data)
		return (EXIT_FAILURE);
	i = 0;
	j = -1;
	cmd = data->commands;
	while (cmd)
	{
		pid[i] = fork ();
		if (pid[i] == -1)
			return (EXIT_FAILURE);
		if (pid[i] == 0)
		{
			fd2fd(&fd_pipes[2 * i], cmd, data);
			command_executer(cmd, data);
		}
		cmd = cmd->next;
		i++;
	}
	cmd = data->commands;
	while (++j < i)
	{
		parent(&fd_pipes[2 * j], pid[j], data, cmd);
		cmd = cmd->next;
	}
	return (EXIT_SUCCESS);
}
