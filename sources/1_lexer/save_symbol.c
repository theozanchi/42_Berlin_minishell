/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_symbol.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 18:56:47 by tzanchi           #+#    #+#             */
/*   Updated: 2023/10/17 12:21:50 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_double_tokens(char *str)
{
	if (!ft_strchr(SUPP_SYMBOLS, *(str + 1)))
		return (EXIT_SUCCESS);
	else
	{
		if ((*str == '<' && *(str + 1) == '<')
			|| (*str == '>' && *(str + 1) == '>'))
			return (EXIT_SUCCESS);
		else
			return (EXIT_FAILURE);
	}
}

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
		while (*ptr && !ft_isspace(*ptr) && !ft_strchr(SUPP_SYMBOLS, *ptr))
			ptr++;
		*new = new_node(str, ptr, ENV_VAR);
	}
	return (ptr);
}

char	*save_symbol(t_data *data, char *str)
{
	t_token	*new;
	char	*ptr;

	if (check_double_tokens(str))
	{
		ft_printf("token expected, symbol '%c' found", *(str + 1));
		return (NULL);
	}
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
