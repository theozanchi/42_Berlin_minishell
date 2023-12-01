/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 10:11:38 by tzanchi           #+#    #+#             */
/*   Updated: 2023/12/01 18:02:48 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define VALID_CHARACTERS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\
							0123456789_"
#define EXPORT_ERR_FLAGS "minishell: export: no options supported\n"
#define EXPORT_INV_IDEN "minishell: export: '%s': not a valid identifier\n"

/**
 * @brief Extracts the value of the variable
 * 
 * @param str The current argument
 * @return Pointer to the string value (located after the = sign if present) 
 */
static char	*extract_value(char *str)
{
	char	*start;
	char	end_char;
	char	*end;

	start = ft_strchr(str, '=');
	if (!start)
		return (NULL);
	if ((start + 1))
		start++;
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
