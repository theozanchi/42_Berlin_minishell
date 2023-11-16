/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 10:11:38 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/16 10:54:52 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define IDENTIFIERS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_"

static int	is_a_valid_identifier(char *str)
{
	size_t	i;
	char	*equal_sign;
	ssize_t	equal_index;
	char	*identifier;

	equal_sign = ft_strchr(str, '=');
	if (!equal_sign)
		equal_index = -1;
	else
		equal_index = equal_sign - str;
	i = 0;
	if (equal_index == 0)
	{
		ft_printf("export: '%s': not a valid identifier", str);
		return (EXIT_FAILURE);
	}
	while (str[i] && str[i] != '=')
	{
		if (!ft_strchr(IDENTIFIERS, str[i]))
		{
			ft_printf("export: '%s': not a valid identifier");
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

static size_t	get_number_of_new_var(t_commands *c)
{
	size_t	count;
	t_list	ptr;

	count = 0;
	ptr = c->arguments;
	while (ptr)
	{
		ptr = ptr->next;
	}
}

static int	check_options(t_commands *c, t_data *data)
{
	if (c->flags)
	{
		ft_printf("export: no options supported");
		return (EXIT_FAILURE);
	}
}

int	builtin_export(t_commands *c, t_data *data)
{
	
}
