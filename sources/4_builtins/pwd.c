/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 09:47:42 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/16 12:06:23 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define PWD_ERR_FLAGS "minishell: pwd: no options supported\n"
#define PWD_ERR_EXTRA_ARG "minishell: pwd: too many arguments\n"

/**
 * @brief Check that the command does not have any argument or flag
 * 
 * @param c The current command node
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int	check_arg(t_commands *c)
{
	if (c->arguments)
		return (ft_printf_exit_code(PWD_ERR_EXTRA_ARG, EXIT_FAILURE));
	if (c->flags)
		return (ft_printf_exit_code(PWD_ERR_FLAGS, EXIT_FAILURE));
	return (EXIT_SUCCESS);
}

/**
 * @brief Displays the current working directory in fd_out
 * 
 * @param fd_out The output file descriptor
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	builtin_pwd(t_commands *c)
{
	char	*cwd;

	if (check_arg(c))
		return (EXIT_FAILURE);
	cwd = NULL;
	getcwd(cwd, 0);
	if (cwd == NULL)
		return (perror_return_failure("getcwd"));
	ft_putstr_fd(cwd, 1);
	free(cwd);
	cwd = NULL;
	return (EXIT_SUCCESS);
}
