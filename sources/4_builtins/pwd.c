/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 09:47:42 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/20 18:52:53 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define PWD_ERR_FLAGS "minishell: pwd: no options supported\n"
#define PWD_ERR_EXTRA_ARG "minishell: pwd: too many arguments\n"
#define PWD_ERR_PWD_NOT_FOUND "minishell: pwd: PWD not found in env\n"

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
 * @brief Displays the current working directory from the environment
 * 
 * @param c 
 * @param data 
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	builtin_pwd(t_commands *c, t_data *data)
{
	size_t	i;

	if (check_arg(c))
		return (EXIT_FAILURE);
	i = 0;
	while (data->env[i])
	{
		if (!ft_strncmp(data->env[i], "PWD=", 4))
		{
			ft_putstr_fd(&data->env[i][4], 1);
			write(1, "\n", 1);
			return (EXIT_SUCCESS);
		}
		i++;
	}
	return (ft_printf_exit_code(PWD_ERR_PWD_NOT_FOUND, EXIT_FAILURE));
}
