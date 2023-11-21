/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 12:57:02 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/21 13:00:22 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "export.h"

/**
 * @brief Loops through the env object to identify the line that begins by str
 * 
 * @param str The string to look for
 * @param data The main data structure
 * @return The string or NULL if the string is not found
 */
char	*get_str_from_env(char *str, t_data *data)
{
	size_t	i;
	size_t	str_len;

	i = 0;
	str_len = ft_strlen(str);
	while (data->env[i])
	{
		if (!ft_strncmp(str, data->env[i], str_len))
			return (data->env[i]);
		i++;
	}
	return (NULL);
}

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
char	*extract_identifier(char *str)
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
char	*get_and_check_identifier(char *str)
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
