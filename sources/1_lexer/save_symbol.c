/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_symbol.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 18:56:47 by tzanchi           #+#    #+#             */
/*   Updated: 2023/10/30 12:01:45 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Allocates a t_token node depending on the symbol: INPUT for '<',
HERE_DOC for '<<', OUTPUT for '>' and APPEND for '>>'
 * 
 * @param str Pointer to the symbol to save in the user input string
 * @param new Pointer to the new token to populate
 * @return Pointer to the first relevant char after the stored
 */
char	*helper_redirections(char *str, t_token **new)
{
	if (*str == *(str + 1))
	{
		if (*str == '<')
			*new = new_node(NULL, NULL, HERE_DOC);
		else
			*new = new_node(NULL, NULL, APPEND);
		return (str + 2);
	}
	else
	{
		if (*str == '<')
			*new = new_node(NULL, NULL, INPUT);
		else
			*new = new_node(NULL, NULL, OUTPUT);
		return (str + 1);
	}
}

/**
 * @brief Analyzes the character located after the dollar that begins the string
 * 
 * @param str Pointer to the symbol to save in the user input string
 * @param new Pointer to the new token to populate
 * @return Pointer to the first relevant char after the stored symbol or NULL
 */
char	*helper_dollar_sign(char *str, t_token **new)
{
	char	*ptr;

	if (*(str + 1) == '?')
	{
		ptr = str + 2;
		*new = new_node(NULL, NULL, EXIT_STATUS);
	}
	else
	{
		ptr = str + 1;
		while (*ptr && !ft_isspace(*ptr) && !ft_strchr(SUPPORTED_SYMBOLS, *ptr))
			ptr++;
		*new = new_node(str + 1, ptr, ENV_VAR);
	}
	return (ptr);
}

/**
 * @brief Creates a new node in the tokens list for the first one or two
symbol(s) contained in the string 'str'
 * 
 * @param data Main data structure of type t_data
 * @param str Pointer to the symbol to save in the user input string
 * @return Pointer to the first relevant char after the stored symbol or NULL
 */
char	*save_symbol(t_data *data, char *str)
{
	t_token	*new;
	char	*ptr;

	if (check_end_of_string(str) || check_double_tokens(str))
		return (NULL);
	if (*str == '<' || *str == '>')
		ptr = helper_redirections(str, &new);
	if (*str == '|')
	{
		ptr = str + 1;
		new = new_node(NULL, NULL, PIPE);
	}
	if (*str == '$')
		ptr = helper_dollar_sign(str, &new);
	if (!new)
		return (NULL);
	ft_tokenlst_addback(data, new);
	return (ptr);
}
