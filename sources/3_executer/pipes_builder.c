/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_builder.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschott <jschott@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 17:47:13 by jschott           #+#    #+#             */
/*   Updated: 2023/11/21 17:29:58 by jschott          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief creates all necessary pipes based on number of commands in a pipeline
/// @param fd_out file descriptor of output of command pipeline
/// @param fd_in file descriptor of output of command pipeline
/// @param cmds_num number of commands in the pipeline
/// @return pointer to the pipes created
int	*build_pipes(int fd_out, int fd_in, int cmds_num)
{
	int	*fd_pipes;
	int	i;

	if (!cmds_num)
		return (0);
	fd_pipes = (int *) ft_calloc((2 * cmds_num) + 2, sizeof(int));
	if (!fd_pipes)
	{
		ft_putstr_fd("minishell: couldn't malloc pipes array\n", 2);
		return (0);
	}
	fd_pipes[0] = fd_in;
	// fd_pipes[1] = fd_in;
	// fd_pipes[2 * cmds_num] = fd_out;
	fd_pipes[(2 * cmds_num) + 1] = fd_out;
	i = 0;
	while (fd_pipes && ++i < cmds_num)
	{
		if (pipe(&fd_pipes[i * 2]) < 0)
		{
			ft_putstr_fd("minishell: error while piping\n", 2);
			free (fd_pipes);
		}
		close_fd(fd_pipes[(i * 2) + 3]);
	}
	return (fd_pipes);
}
