/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 10:11:38 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/20 20:48:12 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define VALID_CHARACTERS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\
							0123456789_"
#define EXPORT_ERR_FLAGS "minishell: export: no options supported\n"
#define EXPORT_INV_IDEN "minishell: export: '%s': not a valid identifier\n"

char	*extract_value(char *str)
{
	char	*start;
	char	end_char;
	char	*end;

	start = ft_strchr(str, '=') + 1;
	if (*start == '\'' || *start == '\"')
		end_char = *start++;
	else
		end_char = '\0';
	end = ft_strrchr(start, end_char);
	return (ft_substr(start, 0, end - start));
}

/**
 * @brief Extracts the identifier from the export argument, i.e. all characters
 * located before the '=' sign
 * 
 * @param str The argument passed to the builtin
 * @return Pointer to the substring, which has been malloc'ed or NULL in case of
 * error or if no equal sign is present
 */
static char	*extract_identifier(char *str)
{
	size_t	identifier_length;
	char	*identifier;

	identifier_length = 0;
	while (str[identifier_length] && str[identifier_length] != '=')
		identifier_length++;
	if (!identifier_length)
	{
		ft_printf(EXPORT_INV_IDEN, str);
		return (NULL);
	}
	identifier = ft_substr(str, 0, identifier_length + 1);
	return (identifier);
}

/**
 * @brief Extracts the identifier from the argument and checks its validity: only
 * lower and upper letters, or underscore signs
 * 
 * @param str The arguments passed to the builtin
 * @return Pointer to the identifier, or NULL if invalid
 */
static char	*get_and_check_identifier(char *str)
{
	char	*identifier;
	size_t	i;

	if (!ft_strchr(str, '='))
		return (NULL);
	identifier = extract_identifier(str);
	if (!identifier)
		return (NULL);
	i = 0;
	while (identifier[i] && identifier[i] != '=')
	{
		if (!ft_strchr(VALID_CHARACTERS, identifier[i]))
		{
			ft_printf(EXPORT_INV_IDEN, identifier);
			free(identifier);
			return (NULL);
		}
		i++;
	}
	return (identifier);
}

/**
 * @brief If an environment variable is already in the env array, it is
 * overwritten by the new value
 * 
 * @param old The old env variable
 * @param new The new env variable
 */
static void	overwrite_env_variable(t_data *data, char *identifier, char *new)
{
	size_t	i;
	size_t	identifier_length;

	i = 0;
	identifier_length = ft_strlen(identifier);
	while (ft_strncmp(data->env[i], identifier, identifier_length))
		i++;
	free(data->env[i]);
	data->env[i] = ft_strdup(new);
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
static int	add_variable_to_env(char *id, char *value, t_data *data)
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
		if (!identifier)
		{
			ptr = ptr->next;
			continue ;
		}
		value = extract_value(ptr->value);
		if (!value)
		{
			free(identifier);
			identifier = NULL;
			ptr = ptr->next;
			continue ;
		}
		if (get_str_from_env(identifier, data))
			overwrite_env_variable(data, identifier, value);
		else
			add_variable_to_env(identifier, value, data);
		free(identifier);
		identifier = NULL;
		free(value);
		value = NULL;
		ptr = ptr->next;
	}
	return (EXIT_SUCCESS);
}
