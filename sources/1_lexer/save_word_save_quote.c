/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_word_save_quote.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 18:55:39 by tzanchi           #+#    #+#             */
/*   Updated: 2023/10/18 17:51:36 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*Creates a new node to save the first word contained in the string 'str'. If
new_node() fails, NULL is returned. The function returns a ptr to the end of the
word (either the first space after it or the NULL terminator)*/
char	*save_word(t_data *data, char *str)
{
	char	*end;
	t_token	*new;

	end = str;
	while (*end && !ft_isspace(*end) && !ft_strchr(SUPPORTED_SYMBOLS, *end))
		end++;
	new = new_node(str, end, OPERAND);
	if (!new)
		return (NULL);
	ft_tokenlst_addback(data, new);
	return (end);
}

/*Creates a new node to save the quoted string contained in the string 'str',
quote_symbol being the symbol used (' or "). If new_node() fails, NULL is
returned. The function returns a ptr to the end of the quoted string (either the
first space after it or the NULL terminator)*/
char	*save_quote(t_data *data, char *str, char quote_symbol)
{
	char	*end_ptr;
	t_token	*new;

	end_ptr = str;
	while (*end_ptr != quote_symbol)
		end_ptr++;
	if (quote_symbol == '\'')
		new = new_node(str, end_ptr, SINGLE_QUOTE);
	else
		new = new_node(str, end_ptr, DOUBLE_QUOTE);
	if (!new)
		return (NULL);
	ft_tokenlst_addback(data, new);
	return (end_ptr + 1);
}
