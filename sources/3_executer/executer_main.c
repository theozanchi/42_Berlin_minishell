/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschott <jschott@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 16:22:45 by jschott           #+#    #+#             */
/*   Updated: 2023/11/16 13:21:06 by jschott          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief redirects fds to given values and calls function that executes
/// @param fd_out desired filedescriptor for output
/// @param cmd command to execute incl. arguments and flags
/// @param fd_in desired filedescriptor for output
/// @param env environmental variable
void	fd2fd(int fd_out, t_commands *cmd, int fd_in, t_data *data)
{
	if (!cmd || !data)
		write(2, "FDF2FDF ERROR\n", 14);
	dup2(fd_out, 1);
	dup2(fd_in, 0);
	cmd_execute(cmd, data);
}

/**
* @brief counts the elements in a linked list of type t_command 
* @param cmds pointer to the first element of a list
* @return integer of number of elements
*/
int	cmd_count(t_commands *cmds)
{
	int			i;
	t_commands	*ptr;

	i = 0;
	ptr = cmds;
	while (ptr)
	{
		ptr = ptr->next;
		i++;
	}
	return (i);
}

void	close_all_fd(int *fd_pipes)
{
	int	i;

	i = -1;
	if (!fd_pipes)
		return ;
	while (fd_pipes[++i])
	{
		if (fd_pipes[i] > 2)
			close (fd_pipes[i]);
	}
}

int	*create_pipes(int fd_out, int fd_in, int cmds_num)
{
	int	*fd_pipes;
	int	i;

	if (!cmds_num)
		return (0);
	fd_pipes = (int *) ft_calloc((2 * cmds_num) + 3, sizeof(int));
	if (!fd_pipes)
	{
		write(2, "PIPES MALLOC ERROR\n", 18); // ERROR MGMT TBD
		return (0);
	}
	fd_pipes[2 * cmds_num + 2] = '\0';
	fd_pipes[0] = fd_in;
	fd_pipes[1] = fd_in;
	i = 2;
	while (i < 2 * cmds_num)
	{
		pipe(&fd_pipes[i]);
		i += 2;
	}
	fd_pipes[2 * cmds_num] = fd_out;
	fd_pipes[(2 * cmds_num) + 1] = fd_out;
	i = 0;
	return (fd_pipes);
}

int	child_process(int *fd_pipes, pid_t *pid, t_data *data)
{
	int			i;
	t_commands	*cmd;

	if (!fd_pipes || !pid || !data)
		return (EXIT_FAILURE);
	i = 0;
	cmd = data->commands;
	while (cmd)
	{
		pid[i] = fork ();
		if (pid[i] == -1)
			return(write(2, "FORKING_ERROR\n", 14));// ERROR MGMT TBD
		if (pid[i] == 0)
			fd2fd(fd_pipes[(2 * i) + 3], cmd, fd_pipes[2 * i], data);
		if (pid[i] > 0)
		{
			wait (NULL);
			if (fd_pipes[(2 * i)] > 2)
				close (fd_pipes[2 * i]);
			if (fd_pipes[(2 * i) + 3] > 2)
				close (fd_pipes[(2 * i) + 3]);
			++i;
			cmd = cmd->next;
		}
	}
	close_all_fd(fd_pipes);
	return (EXIT_SUCCESS);
}

/**
 * @brief Executes and pipes multiple commands including redirects
 * 
 * @param data main data struct includes all commands & redirects
 * @param cmds_num number of commands to execute
 * @param fd_pipes array with the in/out fds of all pipes
 * @param pid array of all process ids
 * @return 0 if success. -1 on errors
 */
int	executer(t_data *data)
{
	int			cmds_num;
	int			*fd_pipes;
	pid_t		*pid;

	cmds_num = cmd_count(data->commands);
	fd_pipes = create_pipes(data->output.fd, data->input.fd, cmds_num);
	if (!fd_pipes)
		return (EXIT_SUCCESS); // ERROR MGMT TBD
	pid = (pid_t *) ft_calloc (cmds_num + 1, sizeof(pid_t));
	if (!pid)
	{
		free (fd_pipes);
		return (write(2, "EXEC MALLOC ERROR\n", 18)); // ERROR MGMT TBD
	}
	child_process(fd_pipes, &pid[1], data);
	free (fd_pipes);
	free (pid);
	dup2(0, 0);
	dup2(1, 1);
	return (EXIT_SUCCESS);
}
