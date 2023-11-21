/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 10:11:38 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/21 20:38:06 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "export.h"

/**
 * @brief If an environment variable is already in the env array, it is
 * overwritten by the new value
 * 
 * @param old The old env variable
 * @param new The new env variable
 */
void	overwrite_env_variable(char *id, char *value, t_data *data)
{
	size_t	i;
	size_t	id_length;

	i = 0;
	id_length = ft_strlen(id);
	while (ft_strncmp(data->env[i], id, id_length))
		i++;
	free(data->env[i]);
	data->env[i] = ft_concat(2, id, value);
	if (!data->env[i])
		perror("minishell: export: malloc");
}

/**
 * @brief Loops through the env array, if the variable is foundm it is
 * overwritten, if not it is added at the end o the array
 * 
 * @param identifier The identifier of the variable
 * @param str The new value to store in env
 * @param data The main data structure
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	add_variable_to_env(char *id, char *value, t_data *data)
{
	size_t	length;
	char	**new;
	size_t	i;

	length = 0;
	while (data->env[length])
		length++;
	new = malloc((length + 2) * sizeof(char *));
	if (!new)
		return (EXIT_FAILURE);
	i = 0;
	while (i < length)
	{
		new[i] = ft_strdup(data->env[i]);
		if (!new[i])
			return (reverse_free_char_array(new, i, EXIT_FAILURE));
		i++;
	}
	new[length] = malloc((ft_strlen(id) + ft_strlen(value) + 1) * sizeof(char));
	ft_strlcpy(new[length], id, ft_strlen(id) + 1);
	ft_strlcpy(&new[length][ft_strlen(id)], value, ft_strlen(value) + 1);
	new[length + 1] = NULL;
	free_char_array(data->env);
	data->env = new;
	return (EXIT_SUCCESS);
}

/**
 * @brief Loops through the arguments of a t_command node, and if they are valid
 * identifiers, adds them to the env array in the main data structure
 * 
 * @param c The current command node
 * @param data The main data structure
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	builtin_export(t_commands *c, t_data *data)
{
	t_list	*ptr;
	char	*identifier;
	char	*value;

	if (c->flags)
		return (ft_printf_exit_code(EXPORT_ERR_FLAGS, EXIT_FAILURE));
	ptr = c->arguments;
	while (ptr)
	{
		identifier = get_and_check_identifier(ptr->value);
		value = extract_value(ptr->value);
		if (!identifier || !value)
		{
			free_and_set_to_null(2, identifier, value);
			ptr = ptr->next;
			continue ;
		}
		if (ft_getenv(identifier, data))
			overwrite_env_variable(identifier, value, data);
		else
			add_variable_to_env(identifier, value, data);
		free_and_set_to_null(2, identifier, value);
		ptr = ptr->next;
	}
	return (EXIT_SUCCESS);
}
