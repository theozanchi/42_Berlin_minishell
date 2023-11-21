/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:10:33 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/21 20:47:39 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define ENV_ERR_FLAGS "minishell: env: no options supported\n"
#define ENV_ERR_EXTRA_ARG "minishell: env: no arguments supported\n"

/**
 * @brief Checks that no flag and no arguments are passed to the builtin
 * 
 * @param c The current command node
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int	check_arg(t_commands *c)
{
	if (c->flags)
		return (ft_printf_exit_code(ENV_ERR_FLAGS, EXIT_FAILURE));
	if (c->arguments)
		return (ft_printf_exit_code(ENV_ERR_EXTRA_ARG, EXIT_FAILURE));
	return (EXIT_SUCCESS);
}

/**
 * @brief Displays the environment variable in the terminal
 * 
 * @param c The current command node
 * @param data The main data structure
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	builtin_env(t_commands *c, t_data *data)
{
	size_t	i;

	if (check_arg(c))
		return (EXIT_FAILURE);
	i = 0;
	while (data->env[i])
		ft_printf("%s\n", data->env[i++]);
	return (EXIT_SUCCESS);
}
