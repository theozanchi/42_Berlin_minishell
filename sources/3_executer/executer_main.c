/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschott <jschott@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 16:22:45 by jschott           #+#    #+#             */
/*   Updated: 2023/11/15 15:18:46 by jschott          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "executer.h"
#include "minishell.h"

void	fd2fd(int fd_out, t_commands *cmd, int fd_in, char **env)
{
	if (!cmd || !env)
		write(2, "FDF2FDF ERROR\n", 14);
// printf("my pipes: %i | %i\n", fd_out, fd_in);
	write(2, "reading from: ", 14);
	write(2, ft_itoa(fd_in), 1);
	write(2, "\n", 1);
	write(2, "writing to:   ", 14);
	write(2, ft_itoa(fd_out), 1);
	write(2, "\n\n", 2);
	dup2(fd_out, 1);
	dup2(fd_in, 0);
	cmd_execute(cmd, env);
	exit (EXIT_SUCCESS);
}

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

/**
 * @brief Executes and pipes multiple commands including redirects
 * 
 * @param data main data struct includes all commands & redirects
 * @param i index
 * @param cmds_num number of commands to execute
 * @param fd_pipes array with the in/out fds of all pipes
 * @param pid array of all process ids
 * @param cmds pointer to the current command to execute
 * @return 0 if success. -1 if arr is NULL
 */

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
	fd_pipes[2 * cmds_num + 2] = '\0'; // Probalbly don't need this one
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
// printf ("fd_pipes:");
// while (i <= (2 * cmds_num) + 1)
// 	printf(" | %i", fd_pipes[i++]);
// printf("\n");
	return (fd_pipes);
}

void	child_process(int *fd_pipes, pid_t *pid, t_data *data)
{
	int			i;
	t_commands	*cmd;

	if (!fd_pipes || !pid || !data)
		return ;
	i = 0;
	cmd = data->commands;
	while (cmd)
	{
		pid[i] = fork ();
		if (pid[i] == -1)
			write(2, "FORKING_ERROR\n", 14);// ERROR MGMT TBD
		if (pid[i] == 0)
			fd2fd(fd_pipes[(2 * i) + 3], cmd, fd_pipes[2 * i], data->env);
		if (pid[i] > 0)
		{
			wait (NULL);
			close (fd_pipes[2 * i]);
			close (fd_pipes[(2 * i) + 3]);
		}
		++i;
		cmd = cmd->next;
	}
	exit (EXIT_SUCCESS);
}

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
	pid[0] = fork();
	if (pid[0] == -1)
		return (write(2, "FORKING_ERROR\n", 14)); // ERROR MGMT TBD
	if (pid[0] == 0)
		child_process(fd_pipes, &pid[1], data);
	if (pid[0] > 1)
	{
		wait (NULL);
		close_all_fd(fd_pipes);
		free (fd_pipes);
		free (pid);
		dup2(0, 0);
		dup2(1, 1);
	}
	return (EXIT_SUCCESS);
}
