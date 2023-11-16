/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 10:11:38 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/16 13:02:08 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define VALID_CHARACTERS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_"
#define EXPORT_ERR_FLAGS "minishell: export: no options supported\n"
#define EXPORT_INV_IDEN "minishell: export: '%s': not a valid identifier\n"

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
	identifier = ft_substr(str, 0, identifier_length);
	return (identifier);
}

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
	while (identifier[i])
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

static void	overwrite_env_variable(char **old, char *new)
{
	free(*old);
	*old = ft_strdup(new);
	if (!(*old))
		perror("minishell: export: malloc");
}

static int	add_variable_to_env(char *identifier, char *str, t_data *data)
{
	size_t	length;
	char	**new_env;

	length = 0;
	while (data->env[length])
	{
		if (!ft_strncmp(identifier, data->env[length], ft_strlen(identifier)))
		{
			overwrite_env_variable(&data->env[length], str);
			return (EXIT_SUCCESS);
		}
		length++;
	}
	new_env = malloc((length + 2) * sizeof(char *));
	if (!new_env)
		return (EXIT_FAILURE);
	ft_memcpy(new_env, data->env, length * sizeof(char *));
	new_env[length] = ft_strdup(str);
	new_env[length + 1] = NULL;
	free_char_array(data->env);
	data->env = new_env;
	return (EXIT_SUCCESS);
}

int	builtin_export(t_commands *c, t_data *data)
{
	t_list	*ptr;
	char	*identifier;

	if (c->flags)
		return (ft_printf_exit_code(EXPORT_ERR_FLAGS, EXIT_FAILURE));
	ptr = c->arguments;
	while (ptr)
	{
		identifier = get_and_check_identifier(ptr->value);
		if (identifier)
		{
			if (add_variable_to_env(identifier, ptr->value, data))
			{
				free (identifier);
				identifier = NULL;
				return (EXIT_FAILURE);
			}
			free(identifier);
			identifier = NULL;
		}
		ptr = ptr->next;
	}
	return (EXIT_SUCCESS);
}
