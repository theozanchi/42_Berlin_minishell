/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 13:11:23 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/16 13:27:11 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define UNSET_ERR_FLAGS "minishell: unset: no options supported\n"

static int	remove_variable_from_env(char *identifier, t_data *data)
{
	size_t	length;
	ssize_t	identifier_index;
	char	**new_env;

	length = 0;
	identifier_index = -1;
	while (data->env[length])
	{
		if (!ft_strncmp(identifier, data->env[length], ft_strlen(identifier)))
			identifier_index = length;
		length++;
	}
	if (identifier_index == -1)
		return (EXIT_SUCCESS);
	new_env = malloc((length) * sizeof(char *));
	if (!new_env)
		return (EXIT_FAILURE);
	ft_memcpy(new_env, data->env, (identifier_index - 1) * sizeof(char *));
	ft_memcpy(new_env + identifier_index, data->env + identifier_index + 1,
		(length - identifier_index) * sizeof(char *));
	new_env[length] = NULL;
	free_char_array(data->env);
	data->env = new_env;
	return (EXIT_SUCCESS);
}

int	builtin_unset(t_commands *c, t_data *data)
{
	t_list	*ptr;

	if (c->flags)
		return (ft_printf_exit_code(UNSET_ERR_FLAGS, EXIT_FAILURE));
	if (!c->arguments)
		return (EXIT_SUCCESS);
	ptr = c->arguments;
	while (ptr)
	{
		if (remove_variable_from_env(ptr->value, data))
			return (EXIT_FAILURE);
		ptr = ptr->next;
	}
	return (EXIT_SUCCESS);
}
