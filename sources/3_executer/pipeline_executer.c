/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_executer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschott <jschott@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 17:54:35 by jschott           #+#    #+#             */
/*   Updated: 2023/11/16 18:14:31 by jschott          ###   ########.fr       */
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
void	fd2fd(int fd_out, t_commands *cmd, int fd_in, t_data *data)
{
	if (!cmd || !data)
		write(2, "FDF2FDF ERROR\n", 14);
	dup2(fd_out, 1);
	dup2(fd_in, 0);
	command_executer(cmd, data);
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
	int			status;
	t_commands	*cmd;

	if (!fd_pipes || !pid || !data)
		return (EXIT_FAILURE);
	i = 0;
	cmd = data->commands;
	while (cmd)
	{
		// printf("current wstatus: %i\n", data->wstatus);
		pid[i] = fork ();
		if (pid[i] == -1)
			return (EXIT_FAILURE);
		if (pid[i] == 0)
			fd2fd(fd_pipes[(2 * i) + 3], cmd, fd_pipes[2 * i], data);
		if (pid[i] > 0)
		{
			// wait (NULL);
			waitpid(pid[i], &status, 0);
			data->wstatus = WEXITSTATUS(status);
			// printf("new status: %i\n", status);
			// printf("%s\n", strerror(255));
			// printf("new wstatus: %i\n\n\n", data->wstatus);
			close_fd (fd_pipes[2 * i]);
			close_fd (fd_pipes[(2 * i) + 3]);
			++i;
			cmd = cmd->next;
		}
	}
	return (EXIT_SUCCESS);
}
