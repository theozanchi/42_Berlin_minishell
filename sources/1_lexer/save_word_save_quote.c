/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_word_save_quote.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 18:55:39 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/13 20:51:49 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Creates a new token to save the first word in 'str'
 * 
 * @param data Main data structure of type t_data
 * @param str Pointer to the beginning of the word in user input
 * @return Pointer to the first relevant char after the stored word or NULL
 */
char	*save_word(t_data *data, char *str)
{
	char	*end;
	t_token	*new;

	end = str;
	while (*end && !ft_isspace(*end) && !ft_strchr(SUPPORTED_SYMBOLS, *end))
		end++;
	new = new_token(str, end, OPERAND, NONE);
	if (!new)
		return (NULL);
	ft_tokenlst_addback(data, new);
	return (end);
}

/**
 * @brief Creates a new node to save the quoted string in 'str'
 * 
 * @param data Main data structure of type t_data
 * @param str Pointer to the first char after quote_symbol
 * @param quote_symbol \' or \"
 * @return Pointer to the first relevant char after the stored word or NULL
 */
char	*save_quote(t_data *data, char *str, char quote_symbol)
{
	char	*end_ptr;
	t_token	*new;

	end_ptr = str;
	while (*end_ptr != quote_symbol)
		end_ptr++;
	if (quote_symbol == '\'')
		new = new_token(str, end_ptr, OPERAND, SGL);
	else
		new = new_token(str, end_ptr, OPERAND, DBL);
	if (!new)
		return (NULL);
	ft_tokenlst_addback(data, new);
	return (end_ptr + 1);
}
