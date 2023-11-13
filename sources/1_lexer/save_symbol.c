/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_symbol.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 18:56:47 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/13 15:31:15 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Identifies the redirection symbol at the beginning of str between <,
<<, > and >>
 * 
 * @param str Pointer to the first char of the redirection symbol
 * @param type Pointer to the t_type value to populate with type
 * @return Pointer to the first relevant char after the redirection symbol
 */
char	*identify_redirection_type(char *str, t_type *type)
{
	if (*str == *(str + 1))
	{
		if (*str == '<')
			*type = HERE_DOC;
		else
			*type = APPEND;
		return (str + 2);
	}
	else
	{
		if (*str == '<')
			*type = INPUT;
		else
			*type = OUTPUT;
		return (str + 1);
	}
}

/**
 * @brief Stores a redirection symbol of the word after in a token in the list
 * 
 * @param str Pointer to the first char of the redirection symbol
 * @param new Pointer to the new token to populate
 * @return Pointer to the first relevant char after the stored redir. or NULL
 */
char	*lexer_helper_redirections(char *str, t_token **new)
{
	t_type	type;
	char	*start;
	char	*end;

	start = identify_redirection_type(str, &type);
	while (ft_isspace(*start))
		start++;
	end = start;
	while (*end && !ft_isspace(*end) && !ft_strchr(SUPPORTED_SYMBOLS, *end))
		end++;
	*new = new_token(start, end, type);
	return (end);
}

/**
 * @brief Analyzes the character located after the dollar that begins the string
 * 
 * @param str Pointer to the symbol to save in the user input string
 * @param new Pointer to the new token to populate
 * @return Pointer to the first relevant char after the stored symbol or NULL
 */
char	*lexer_helper_dollar_sign(char *str, t_token **new)
{
	char	*ptr;

	if (*(str + 1) == '?')
	{
		ptr = str + 2;
		*new = new_token(NULL, NULL, EXIT_CODE);
	}
	else
	{
		ptr = str + 1;
		while (*ptr && !ft_isspace(*ptr) && !ft_strchr(SUPPORTED_SYMBOLS, *ptr))
			ptr++;
		*new = new_token(str + 1, ptr, ENV_VARIABLE);
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
		ptr = lexer_helper_redirections(str, &new);
	if (*str == '|')
	{
		ptr = str + 1;
		new = new_token(NULL, NULL, PIPE);
	}
	// if (*str == '$')
	// 	ptr = lexer_helper_dollar_sign(str, &new);
	if (!new)
		return (NULL);
	ft_tokenlst_addback(data, new);
	return (ptr);
}
