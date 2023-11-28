/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_executer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschott <jschott@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 17:54:35 by jschott           #+#    #+#             */
/*   Updated: 2023/11/28 10:08:52 by jschott          ###   ########.fr       */
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
 * @brief closes all file desrciptor in an array
 * @param fd_pipes array with file descriptors
*/
void	close_unused_fd(int *fd_array, int fd_in, int fd_out, int len)
{
	int	i;
	int	*fd_pipes;

	i = 1;
	fd_pipes = fd_array;
	// ft_putstr_fd("Closing:  ", 2);
	// ft_putnbr_fd(fd_pipes[0], 2);
	// ft_putnbr_fd(fd_pipes[1], 2);
	// ft_putstr_fd(" ", 2);
	while (++i < len)
	{
		if (i > 0 && i != fd_in && i != fd_out)
		{
			// ft_putchar_fd('X', 2);
			close_fd (&fd_pipes[i]);
		}
		// else
		// 	ft_putnbr_fd(fd_pipes[i], 2);
	}
	// ft_putstr_fd(" ", 2);
	// ft_putnbr_fd(fd_pipes[i], 2);
	// ft_putnbr_fd(fd_pipes[++i], 2);
	// ft_putstr_fd("\n", 2);
}

void	fd_printer(int *fd_pipes, int readlen, int index)
{
	int	*fds;
	int i = -1;

	fds = fd_pipes;
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
	close_unused_fd(fd_pipes, pos + 3, pos + 3, (2 * cmd_count(data->commands)));
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
	// int		original_fd[2];

	// original_fd[0] = dup(0);
	// original_fd[1] = dup(1);
	// fd_printer(fd_pipes, 2 + (2 * cmd_count(data->commands)), 0);
	pid = fork();
	if (pid < 0)
		return (EXIT_FAILURE);
	if (pid == 0)
	{
		dup2(fd_pipes[pos], 0);
		dup2(fd_pipes[pos + 3], 1);
		close_unused_fd(fd_pipes, pos, pos + 3, (2 * cmd_count(data->commands)));
		exit (command_executer(cmd, data));
	}
	// close_unused_fd(&fd_pipes[pos], 2, 2, 4);
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
	pid_t		*wpid;
	int			i;

	if (!fd_pipes || !pid || !data)
		return (EXIT_FAILURE);
	i = 0;
	// fd_printer(fd_pipes, 2 + (2 * cmd_count(data->commands)), 1); //DEBUGGING
	cmd = data->commands;
	while (cmd)
	{
		// fd_printer(fd_pipes, 2 + (2 * cmd_count(data->commands)), 0);
		if (cmd_is_a_builtin(cmd))
		{
			execute_builtin (fd_pipes, i *2, cmd, data);
			pid[i] = 1;
		}
		else
		{
			execute_env (fd_pipes, i *2, cmd, data);
			close_fd(&fd_pipes[(i * 2)]);
		}
		i++;
		cmd = cmd->next;
	}
	wpid = (pid_t *) ft_calloc (i, sizeof(pid_t));
/* 	while (--i >= 0)
	{
		if (pid[i] > 0)
		{
			wpid[i] = fork();
			if (wpid[i] == -1)
				return (EXIT_FAILURE);
			if (wpid[i] == 0)
			{
				waitpid(pid[i], 0, 0);
				close_fd (&fd_pipes[(2 * i)]);
				close_fd (&fd_pipes[(2 * i) + 3]);
				exit (EXIT_SUCCESS);
			}
		}
		else
			wpid[i] = 0;
	}
	i = cmd_count(data->commands);
	while (--i >= 0)
	{
		if (wpid[i] > 0)
		{
			waitpid(pid[i], 0, 0);
			fd_pipes[(2 * i) + 2] = -1;
		}
	} */
	int j = -1;
	while (++j < i)
	{
		// ft_putnbr_fd(j, 2);
		// ft_putstr_fd(": ", 2);
		// ft_putnbr_fd(pid[j], 2);
		if (pid[j] == 0)
		{
			waitpid(pid[j], 0, 0);
			close_fd(&fd_pipes[(j * 2)]);
			close_fd(&fd_pipes[(j * 2) + 3]);
		}
	}
	free (wpid);
	// ft_putstr_fd("\n\n\nDONE\n", 2);
	// ft_putstr_fd("\nFINAL FDS:", 2);
	// fd_printer(fd_pipes, 2 + (2 * cmd_count(data->commands)), 1); //DEBUGGING
	return (EXIT_SUCCESS);
}
