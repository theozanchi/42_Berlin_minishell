/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 17:47:56 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/15 18:09:49 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief echo displays a string of text. The -n options is available:do not
 * output a newline
 * 
 * @param c The current command node
 * @param fd_out The file descriptor in which to write
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	builtin_echo(t_commands *c, int fd_out)
{
	t_list	*ptr;

	if (c->flags && (c->flags->next || ft_strcmp(c->flags->value, "-n")))
		return (ft_printf_exit_code("Only -n is supported", EXIT_FAILURE));
	ptr = c->arguments;
	while (ptr)
	{
		ft_putstr_fd(ptr->value, fd_out);
		ptr = ptr->next;
	}
	if (c->flags && ft_strcmp(c->flags->value, "-n"))
		ft_putstr_fd("\n", fd_out);
	return (EXIT_SUCCESS);
}
