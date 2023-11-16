/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 10:56:06 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/16 11:05:58 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	builtin_exit(t_commands *c, t_data *data)
{
	if (check_arg(c, data))
		return (EXIT_FAILURE);
	if (c->arguments)
		exit_minishell(data, ft_atoi(c->arguments->value));
	else
		exit_minishell(data, EXIT_SUCCESS);
}
