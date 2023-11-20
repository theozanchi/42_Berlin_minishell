/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 17:47:56 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/20 19:22:42 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define ECHO_ERR_FLAGS "minishell: cd: only -n is supported\n"

/**
 * @brief Checks that argument is valid: only -n flag is supported
 * 
 * @param c The current command node
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int	check_arg(t_commands *c)
{
	if (!c->flags)
		return (EXIT_SUCCESS);
	if (c->flags && (c->flags->next || ft_strcmp(c->flags->value, "-n")))
		return (ft_printf_exit_code(ECHO_ERR_FLAGS, EXIT_FAILURE));
	return (EXIT_SUCCESS);
}

/**
 * @brief echo displays a string of text. The -n options is available:do not
 * output a newline
 * 
 * @param c The current command node
 * @param fd_out The file descriptor in which to write
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	builtin_echo(t_commands *c)
{
	t_list	*ptr;

	if (check_arg(c))
		return (EXIT_FAILURE);
	ptr = c->arguments;
	while (ptr)
	{
		ft_putstr_fd(ptr->value, 1);
		if (ptr->next)
			ft_putstr_fd(" ", 1);
		ptr = ptr->next;
	}
	if (!c->flags)
		ft_putstr_fd("\n", 1);
	return (EXIT_SUCCESS);
}
