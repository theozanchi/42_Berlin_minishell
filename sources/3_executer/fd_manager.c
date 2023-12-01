/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_manager.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschott <jschott@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 11:14:09 by jschott           #+#    #+#             */
/*   Updated: 2023/12/01 12:55:47 by jschott          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief closes a given file descriptor, that is neither stdin, stdout or stderr
 * @param fd file descriptor
 * @return 0 on success, 1 on failure
 */
int	close_fd(int *fd)
{
	if (*fd <= 2)
		return (EXIT_SUCCESS);
	if (close(*fd) == -1)
	{
		ft_putendl_fd("minishell: error while closing fd", 2);
		return (EXIT_FAILURE);
	}
	*fd = -1;
	return (EXIT_SUCCESS);
}

/**
 * @brief closes all file unecessary desriptors in an array of fds, 
	based on a start position and a variable that signals which fds stay open.
 * @param fd_pipes array with all file descriptors
 * @param pos position in fd_pipes of the in- & output pipe file descriptors
 * @param keep variable that signals which pipe ends need to stay open
 * @param len length of fd_pipes
 * @return 
*/
int	close_unused_fd(int *fd_pipes, int pos, int keep, int len)
{
	int	i;
	int	*ptr;
	int	fd_in;
	int	fd_out;

	if (!fd_pipes || pos < 0 || len == 0 || keep > FDX_RW || keep < FDX_NONE)
		return (EXIT_FAILURE);
	i = 1;
	ptr = fd_pipes;
	fd_in = -1;
	fd_out = -1;
	if (keep == FDX_RW || keep == FDX_OR)
		fd_in = pos;
	if (keep == FDX_RW || keep == FDX_OW)
		fd_out = pos + 3;
	while (++i < len)
	{
		if (i != fd_in && i != fd_out \
			&& close_fd (&ptr[i]) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
