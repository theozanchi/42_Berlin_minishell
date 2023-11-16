/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 09:47:42 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/16 10:09:30 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Displays the current working directory in fd_out
 * 
 * @param fd_out The output file descriptor
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	builtin_pwd(int fd_out)
{
	char	*cwd;

	cwd = NULL;
	getcwd(cwd, 0);
	if (cwd == NULL)
		return (EXIT_FAILURE);
	ft_putstr_fd(cwd, fd_out);
	free(cwd);
	cwd = NULL;
	return (EXIT_SUCCESS);
}
