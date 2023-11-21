/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 12:57:02 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/21 20:39:02 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "export.h"

/**
 * @brief Extracts the value of the variable
 * 
 * @param str The current argument
 * @return Pointer to the string value (located after the = sign if present) 
 */
char	*extract_value(char *str)
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
