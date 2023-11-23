/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_executer BACKUP.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschott <jschott@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 17:54:35 by jschott           #+#    #+#             */
/*   Updated: 2023/11/23 16:07:26 by jschott          ###   ########.fr       */
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
	return (EXIT_SUCCESS);
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
	close_fd(fd_pipes[1]);
	if (!cmd_is_a_builtin(cmd))
		close_fd(fd_pipes[2]);
	if (!cmd || !data)
		write(2, "FDF2FDF ERROR\n", 14);
	dup2(fd_pipes[3], 1);
	dup2(fd_pipes[0], 0);
}

/**
 * @brief Waits for a given childprocess of pid, saves its exec-code to data
 * 			and closes pipes, used by child
 * @param fd_out fd of the output pipe
 * @param fd_in fd of input pipe
 * @param pid process id of child to wait to exit
 * @param data  main datastructure
*/
void	parent(int *fd_pipes, pid_t pid, t_data *data, int i)
{
	int	status;
	int	fd_in;
	int	fd_out;

	if (pid == -1)
		return ;
	fd_in = fd_pipes[0];
	fd_out = fd_pipes[3];
	close_fd (fd_in);
	close_fd (fd_out);
	ft_putstr_fd("Parent waiting for child #", 2);
	ft_putnbr_fd(i, 2);
	ft_putendl_fd("", 2);
	waitpid(pid, &status, 0);
	data->wstatus = WEXITSTATUS(status);
	ft_putstr_fd("Parent found child #", 2);
	ft_putnbr_fd(i, 2);
	ft_putendl_fd("\n", 2);
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
	int			orig_fdin;
	int			orig_fdout;

	if (!fd_pipes || !pid || !data)
		return (EXIT_FAILURE);
	i = 0;
	j = -1;
	cmd = data->commands;
	while (cmd)
	{
		if (!cmd_is_a_builtin(cmd))
		{
			pid[i] = fork ();
			if (pid[i] == -1)
				return (EXIT_FAILURE);
			if (pid[i] == 0)
			{
				fd2fd(&fd_pipes[2 * i], cmd, data);
				// sleep(1);
				// exit (EXIT_SUCCESS);
				command_executer(cmd, data);
				exit (EXIT_SUCCESS);
			}
		}
		else
		{
			pid[i] = -1; //Setting to -1 to skip this pid in parent later
			orig_fdin = dup(0); // saving stdin to reset it later to this value
			orig_fdout = dup(1);
			fd2fd(&fd_pipes[2 * i], cmd, data); // redirecting stdin&stdout
			data->wstatus = launch_builtin(cmd, data); // save exit value
			close_fd(fd_pipes[2 * i]); // closing used fds after execution
			close_fd(fd_pipes[(2 * i) + 3]); // **
			fd_pipes[2 * i] = -1; // set used to -1, so they won't get used again
			fd_pipes[(2 * i) + 3] = -1;
			dup2(orig_fdout, 1);
			dup2(orig_fdin, 0);
		}
		cmd = cmd->next;
		i++;
	}

	while (--i >= 0)
		parent(&fd_pipes[2 * i], pid[i], data, i);
	// sleep (1);
	// while (++j < i)
	// 	parent(&fd_pipes[2 * j], pid[j], data, j);
	ft_putstr_fd("DONE\n", 2);
	return (EXIT_SUCCESS);
}
