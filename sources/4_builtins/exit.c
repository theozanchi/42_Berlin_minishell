/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 10:56:06 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/16 11:10:03 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check that the argument for the exit function are valid: one max,
 * numeric
 * 
 * @param c The current command node
 * @param data The main data structure
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int	check_arg(t_commands *c, t_data *data)
{
	if (c->flags)
	{
		ft_printf("exit: no options supported");
		return (EXIT_FAILURE);
	}
	if (c->arguments)
	{
		if (c->arguments->next)
		{
			ft_printf("minishell: exit: too many arguments");
			return (EXIT_FAILURE);
		}
		if (!ft_isnumeric(c->arguments->value))
		{
			ft_printf("minishell: exit: numeric argument required");
			return (EXIT_FAILURE);
		}
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
	if (check_arg(c, data))
		return (EXIT_FAILURE);
	if (c->arguments)
		exit_minishell(data, ft_atoi(c->arguments->value));
	else
		exit_minishell(data, EXIT_SUCCESS);
}
