/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_executer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschott <jschott@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 14:13:26 by jschott           #+#    #+#             */
/*   Updated: 2023/11/24 16:20:00 by jschott          ###   ########.fr       */
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
		return (EXIT_FAILURE);
	close(*fd);
	// *fd = -1;
	return (EXIT_SUCCESS);
}

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

/**
 * @brief closes all file desrciptor in an array
 * @param fd_pipes array with file descriptors
*/
void	close_unused_fd(int *fd_pipes, int x, int len)
{
	int	i;
	int y = x + 3;

	i = -1;
	ft_putstr_fd("Closing:\n", 2);
	while (++i < len)
	{
		if (i > 0 && i != x && i !=y)
		{
			ft_putchar_fd('X', 2);
			close_fd (&fd_pipes[i]);
		}
		else
			ft_putnbr_fd(fd_pipes[i], 2);
		ft_putstr_fd(" ", 2);
	}
	ft_putstr_fd("\n", 2);
}

int	execute_recursive(int *fd_pipes, t_commands *cmd, t_data *data)
{
	pid_t	pid;
	int		status;
	int		len;
	
	ft_putstr_fd("\nMY FDs ", 2);
	ft_putstr_fd(cmd->command, 2);
	fd_printer (fd_pipes, 4, 0);
	
	len = cmd_count(cmd);
	if (cmd->next)
	{
		ft_putstr_fd("\nFDs BEFORE RECURSION", 2);
		ft_putstr_fd(cmd->command, 2);
		fd_printer (fd_pipes, 6, 0);


		execute_recursive(&fd_pipes[2], cmd->next, data);
	}
	close_fd(&fd_pipes[1]);
	// close_fd(&fd_pipes[2]);
	if (!cmd_is_a_builtin(cmd))
	{
		pid = fork();
		if (pid == -1)
			return (EXIT_FAILURE);
		if (pid == 0)
		{
			close_fd(&fd_pipes[1]);
			dup2(fd_pipes[3], 1);
			dup2(fd_pipes[0], 0);

			ft_putstr_fd("\nMY BEFORE EXECUTION ", 2);
			ft_putstr_fd(cmd->command, 2);
			fd_printer (fd_pipes, 4, 0);
			
			// close_unused_fd(fd_pipes, 0, len);
			command_executer(cmd, data);
			exit (EXIT_FAILURE);
		}
		waitpid(pid, &status, 0);

		ft_putstr_fd("\nDONE WAITING FOR ", 2);
		ft_putendl_fd(cmd->command, 2);

		// close_unused_fd(fd_pipes, -1, len);
		close_fd(&fd_pipes[0]);
		// close_fd(&fd_pipes[2]);
		close_fd(&fd_pipes[3]);
		data->wstatus = WEXITSTATUS(status);
	}
	else
	{
		close_fd(&fd_pipes[1]);
		close_fd(&fd_pipes[2]);
		dup2(fd_pipes[3], 1);
		dup2(fd_pipes[0], 0);
		data->wstatus = launch_builtin(cmd, data);
		close_fd(&fd_pipes[0]);
		close_fd(&fd_pipes[3]);	
		// close_unused_fd(fd_pipes, -1, len);	
		// close_fd(&fd_pipes[3]);
		// close_fd(&fd_pipes[0]);
		// close_fd(&fd_pipes[1]);
		// close_fd(&fd_pipes[2]);
	}
	/* if (cmd->next)
	{
		ft_putstr_fd("\nFDs BEFORE RECURSION", 2);
		ft_putstr_fd(cmd->command, 2);
		fd_printer (fd_pipes, 6, 0);


		execute_recursive(&fd_pipes[2], cmd->next, data);
	} */
	return (data->wstatus);
}

int	execute_pipeline(int *fd_pipes, pid_t *pid, t_data *data)
{
	int	exit_code;

	ft_putstr_fd("INITIAL FDs", 2);
	fd_printer (fd_pipes, 2 + (2 * cmd_count(data->commands)), 1);
	exit_code = execute_recursive(fd_pipes, data->commands, data);
	ft_putstr_fd("\nMY EXIT FDs", 2);
	fd_printer (fd_pipes, 2 + (2 * cmd_count(data->commands)), 1);
	return (exit_code);
	free (pid);

}
