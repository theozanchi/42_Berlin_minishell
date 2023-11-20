/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 13:11:23 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/20 19:26:31 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define UNSET_ERR_FLAGS "minishell: unset: no options supported\n"

/**
 * @brief Allocates memory and duplicates the old environment variable while
 * omitting the variable that has to be unset
 * 
 * @param data The main data structure
 * @param new_env The new variable to populate
 * @param length The new length of the env array
 * @param identifier_index The index of the variable to unset
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int	build_new_env(t_data *data, char ***new_env, ssize_t length,
							ssize_t identifier_index)
{
	ssize_t	i;
	ssize_t	j;

	*new_env = malloc((length) * sizeof(char *));
	if (!*new_env)
		return (EXIT_FAILURE);
	i = 0;
	j = 0;
	while (i < length)
	{
		if (i != identifier_index)
		{
			(*new_env)[j] = ft_strdup(data->env[i]);
			if (!(*new_env)[j])
				return (reverse_free_char_array(*new_env, j, EXIT_FAILURE));
			j++;
		}
		i++;
	}
	(*new_env)[j] = NULL;
	return (EXIT_SUCCESS);
}

/**
 * @brief Loops through the old env to locate the variable and unset it. If the
 * variable is not found, EXIT_SUCCESS is returned
 * 
 * @param identifier The identifier of the variable to unset
 * @param data The main data structure
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int	remove_variable_from_env(char *identifier, t_data *data)
{
	size_t	length;
	ssize_t	identifier_index;
	char	**new_env;

	length = 0;
	identifier_index = -1;
	new_env = NULL;
	while (data->env[length])
	{
		if (!ft_strncmp(identifier, data->env[length], ft_strlen(identifier))
			&& data->env[length][ft_strlen(identifier)] == '=')
			identifier_index = length;
		length++;
	}
	if (identifier_index == -1)
		return (EXIT_SUCCESS);
	if (build_new_env(data, &new_env, length, identifier_index))
		return (EXIT_FAILURE);
	free_char_array(data->env);
	data->env = new_env;
	return (EXIT_SUCCESS);
}

/**
 * @brief Loops through the env variable and unset the variables passed as args
 * 
 * @param c The current command node
 * @param data The main data structure
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
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
