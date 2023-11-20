/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 10:56:06 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/20 10:17:29 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define EXIT_ERR_FLAGS "minishell: exit: no options supported\n"
#define EXIT_ERR_EXTRA_ARG "minishell: exit: too many arguments\n"
#define EXIT_ERR_NON_NUM_ARG "minishell: exit: numeric argument required\n"

/**
 * @brief Check that the argument for the exit function are valid: one max,
 * numeric
 * 
 * @param c The current command node
 * @param data The main data structure
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int	check_arg(t_commands *c)
{
	if (c->flags)
		return (ft_printf_exit_code(EXIT_ERR_FLAGS, EXIT_FAILURE));
	if (c->arguments)
	{
		if (c->arguments->next)
			return (ft_printf_exit_code(EXIT_ERR_EXTRA_ARG, EXIT_FAILURE));
		if (!ft_isnumeric(c->arguments->value))
			return (ft_printf_exit_code(EXIT_ERR_NON_NUM_ARG, EXIT_FAILURE));
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief The exit builtin  cause minishell to exit from its current execution
 * environment with the exit status specified by the unsigned decimal integer n
 * passed as an argument or EXIT_SUCCESS if not specified 
 * 
 * @param c The current command node
 * @param data The main data structure
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	builtin_exit(t_commands *c, t_data *data)
{
	if (check_arg(c))
		return (EXIT_FAILURE);
	if (!c->arguments)
		exit_minishell(data, EXIT_SUCCESS);
	else
		exit_minishell(data, ft_atoi(c->arguments->value));
	return (EXIT_SUCCESS);
}
