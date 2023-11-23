/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_executer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschott <jschott@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 17:54:35 by jschott           #+#    #+#             */
/*   Updated: 2023/11/23 15:58:48 by jschott          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief closes a given file descriptor, that is neither stdin, stdout or stderr
 * @param fd file descriptor
 * @return 
 */
int	close_fd(int *fd)
{
	if (*fd <= 2)
		return (EXIT_SUCCESS);
	close(*fd);
	*fd = -1;
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
		close_fd (&fd_pipes[i]);
}

/**
 * @brief redirects fds to given values and calls function that executes
 * @param fd_out desired filedescriptor for output
 * @param cmd command to execute incl. arguments and flags
 * @param fd_in desired filedescriptor for output
 * @param env environmental variable
 */
/* void	fd2fd(int *fd_pipes, t_commands *cmd, t_data *data)
{
	return ;
} */

/**
 * @brief Waits for a given childprocess of pid, saves its exec-code to data
 * 			and closes pipes, used by child
 * @param fd_out fd of the output pipe
 * @param fd_in fd of input pipe
 * @param pid process id of child to wait to exit
 * @param data  main datastructure
*/
/* void	parent(int *fd_pipes, pid_t pid, t_data *data, int i)
{
	return ;
}
 */

void	fd_printer(int *fds, int readlen, int index)
{
	int i = -1;
	if (index)
		ft_putstr_fd("\ni: ", 2);
	while (index && ++i < readlen)
	{
		if (!(i % 2))
			ft_putstr_fd("[", 2);
		if (i < 10 && i >= 0)
			ft_putstr_fd(" ", 2);
		ft_putnbr_fd(i, 1);
		if ((i % 2))
			ft_putstr_fd("]", 2);
		else
			ft_putstr_fd("|", 2);
	}
	i = -1;
	ft_putstr_fd("\nv: ", 2);
	while ( ++i < readlen)
	{
		if (!(i % 2))
			ft_putstr_fd("[", 2);
		if (fds[i] < 10)
			ft_putstr_fd(" ", 2);
		if (fds[i] < 0)
			ft_putchar_fd('x', 2);
		else
			ft_putnbr_fd(fds[i], 2);
		if ((i % 2))
			ft_putstr_fd("]", 2);
		else
			ft_putstr_fd("|", 2);
	}
	ft_putstr_fd("\n", 2);
}

void	execute_builtin(int *fd_in_out, t_commands *cmd, t_data *data)
{
	int	original_fd[2];

	original_fd[0] = dup(0);
	original_fd[1] = dup(1);
	dup2(fd_in_out[0], 0);
	dup2(fd_in_out[3], 1);
	close_fd(&fd_in_out[1]);
	// close_fd(fd_in_out[2]); //THIS EXITS MY PROCESSES
	data->wstatus = launch_builtin(cmd, data);
	close_fd(&fd_in_out[0]);
	close_fd(&fd_in_out[3]);
	dup2(original_fd[0], 0);
	dup2(original_fd[1], 1);
}

void	execute_env(int *fd_in_out, t_commands *cmd, t_data *data)
{
	dup2(fd_in_out[0], 0);
	dup2(fd_in_out[3], 1);
	// close_fd(&fd_in_out[1]);
	// close_fd(&fd_in_out[2]); //THIS EXITS MY PROCESSES
	exit (command_executer(cmd, data));
	exit (EXIT_FAILURE);
}
/* 
void	fetch_children(int **fd_in_out, int *pid, t_data *data)
{
	int	i;
	int	num;
	int exit_status;

	i = -1;
	num = cmd_count(data->commands);
	
	ft_putstr_fd("\nFDS pre child fetch:\n", 2);
	fd_printer(fd_in_out[0], 2 + (2 * cmd_count(data->commands)), 1); //DEBUGGING
	
	while (++i <= num)
	{
		if (pid[i] > 0)
		{
			waitpid(pid[i], &exit_status, 0);
			close_fd (fd_in_out[(2 * i)]);
			close_fd (fd_in_out[(2 * i) + 1]);
			close_fd (fd_in_out[(2 * i) + 2]);
			close_fd (fd_in_out[(2 * i) + 3]);

		}
	}
}
 */
/**
 * @brief executes multiple commands in a pipeline by forking in processes
 * @param fd_pipes array with file descriptors
 * @param pid array with process ids
 * @param data main data structure
 * @return exit code
*/
int	execute_pipeline(int *fd_pipes, pid_t *pid, t_data *data)
{
	t_commands	*cmd;
	int			i;
	int			original_fd[2];

	original_fd[0] = dup(0);
	original_fd[1] = dup(1);
	if (!fd_pipes || !pid || !data)
		return (EXIT_FAILURE);
	i = 0;
	// fd_printer(fd_pipes, 2 + (2 * cmd_count(data->commands)), 1); //DEBUGGING
	cmd = data->commands;
	while (cmd)
	{
		// fd_printer(fd_pipes, 2 + (2 * cmd_count(data->commands)), 0);
		close_fd(&fd_pipes[(i * 2) + 1]);
		fd_pipes[(i * 2) + 1] = -1;
		if (cmd_is_a_builtin(cmd))
		{
			execute_builtin(&fd_pipes[i * 2], cmd, data);
			pid[i] = 0;
		}
		else
		{
			// close_fd(fd_pipes[(i * 2) + 2]);
			// fd_pipes[(i * 2) + 2] = -1;
			pid[i] = fork();
			if (pid[i] == -1)
				return (EXIT_FAILURE);
			if (pid[i] == 0)
				execute_env(&fd_pipes[i * 2], cmd, data);
		}
		i++;
		cmd = cmd->next;
	}
	// dup2(original_fd[0], 0);
	// dup2(original_fd[1], 1);

	// fetch_children(&fd_pipes, pid, data);
	
	// ft_putstr_fd("\nFDS pre child fetch:\n", 2);
	// fd_printer(fd_pipes, 2 + (2 * cmd_count(data->commands)), 1); //DEBUGGING
	i = cmd_count(data->commands);
	while (--i >= 0)
	{
		if (pid[i] > 0)
		{
			waitpid(pid[i], 0, 0);
			close_fd (&fd_pipes[(2 * i)]);
			close_fd (&fd_pipes[(2 * i) + 1]);
			close_fd (&fd_pipes[(2 * i) + 2]);
			close_fd (&fd_pipes[(2 * i) + 3]);

		}
	}
	
	
	// ft_putstr_fd("\n\n\nDONE\n", 2);
	// ft_putstr_fd("\nFINAL FDS:\n", 2);
	// fd_printer(fd_pipes, 2 + (2 * cmd_count(data->commands)), 1); //DEBUGGING
	return (EXIT_SUCCESS);
}
