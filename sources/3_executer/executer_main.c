/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschott <jschott@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 16:22:45 by jschott           #+#    #+#             */
/*   Updated: 2023/11/14 14:16:25 by jschott          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "executer.h"
#include "minishell.h"

void	fd2fd(int fd_out, t_commands *cmd, int fd_in, char **env)
{
	if (!cmd || !env)
		write(2, "FDF2FDF ERROR\n", 14);
	dup2(fd_out, 1);
	dup2(fd_in, 0);
	cmd_execute(cmd, env);
	close (fd_in);
	close (fd_out);
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

int	executer(t_data *data)
{
	int			i;
	int			cmds_num;
	int			*fd_pipes;
	pid_t		*pid;
	t_commands	*cmds;

	i = 0;
	cmds = data->commands;
	write(2, cmds->command, ft_strlen(cmds->command));
		write(2, "\n\n", 2);
	cmds_num = cmd_count(data->commands);
	fd_pipes = (int *) ft_calloc(3 + cmds_num, 2 * sizeof(int));
	if (!fd_pipes)
		write(2, "EXEC MALLOC ERROR\n", 18); // ERROR MGMT TBD
	fd_pipes[cmds_num + 3] = '\0'; // Probalbly don't need this one
	pid = (pid_t *) ft_calloc (cmds_num - 1, sizeof(pid_t));
	if (!pid)
		write(2, "EXEC MALLOC ERROR\n", 18); // ERROR MGMT TBD
	fd_pipes[0] = data->input.fd;
	fd_pipes[1] = data->output.fd;
	i = 0;
	while (cmds->next)
	{
		if (pipe(&fd_pipes[(i * 2) + 2]) < -1)
			write(2, "PIPING ERROR\n", 13); // ERROR MGMT TBD
		pid[i] = fork();
		if (pid[i] == -1)
			write(2, "FORKING ERROR\n", 14); // ERROR MGMT TBD
		if (pid[i] == 0)
		{
//DEBBUGING STUFF
		write(2, "out & in: ", 10);
		write(2, "  ", 2);
		write(2, ft_itoa((i * 2) + 3), 1);
		write(2, ft_itoa(i * 2), 1);
		write(2, "  ", 2);
		write(2, ft_itoa((i * 2) + 1), 1);
		write(2, ft_itoa((i * 2) + 2), 1);
		write(2, "\n\n", 2);
			
			if (i > 0)
				close(fd_pipes[(i * 2) + 1]);
			close(fd_pipes[(i * 2) + 2]);
			fd2fd(fd_pipes[(i * 2) + 3], cmds, fd_pipes[i * 2], data->env);
		}
		if (pid[i] != 0)
		{
			wait (NULL);
			close(fd_pipes[3 + (i * 2)]);
			++i;
			cmds = cmds->next;
		}
	}
	fd2fd (fd_pipes[1], cmds, fd_pipes[i * 2], data->env);
	free (fd_pipes);
	free (pid);
	return (0);
}
