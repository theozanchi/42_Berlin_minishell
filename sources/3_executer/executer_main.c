/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschott <jschott@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 16:22:45 by jschott           #+#    #+#             */
/*   Updated: 2023/12/04 11:52:43 by jschott          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	while (ptr && ++i)
		ptr = ptr->next;
	return (i);
}

/**
 * @brief builds a execution pipeline of multiple commands including in-
 * & output redirect and calls and waits for execution of the pipeline
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
	pid = (pid_t *) ft_calloc (cmds_num, sizeof(pid_t));
	if (!pid)
		return (EXIT_FAILURE);
	fd_pipes = build_pipes(data->output.fd, data->input.fd, cmds_num);
	if (!fd_pipes)
		return (EXIT_FAILURE);
	execute_pipeline(fd_pipes, pid, data);
	free (fd_pipes);
	free (pid);
	return (EXIT_SUCCESS);
}
