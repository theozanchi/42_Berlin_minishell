/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_symbol.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 18:56:47 by tzanchi           #+#    #+#             */
/*   Updated: 2023/10/17 19:09:29 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*Checks if a symbol is at the end of a string. Returns EXIT_FAILURE if needed
and prints an error message*/
int	check_end_of_string(char *str)
{
	if ((*str == '<' || *str == '>') && *str == *(str + 1) && !*(str + 2))
	{
		ft_printf("token expected after symbol '%c%c'\n", *str, *str);
		return (EXIT_FAILURE);
	}
	if (!*(str + 1))
	{
		ft_printf("token expected after symbol '%c'\n", *str);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/*Checks for two consecutive SUPPORTED_SYMBOLS. If two consecutive symbols are
found, only '<<' and '>>' are valid combinations. Returns EXIT_FAILURE in case
of invalid combination and prints an error message*/
int	check_double_tokens(char *str)
{
	if (!ft_strchr(SUPPORTED_SYMBOLS, *(str + 1)))
		return (EXIT_SUCCESS);
	else
	{
		if ((*str == '<' && *(str + 1) == '<')
			|| (*str == '>' && *(str + 1) == '>'))
			return (EXIT_SUCCESS);
		else
		{
			ft_printf("token expected after symbol '%c', '%c' found\n",
				*str, *(str + 1));
			return (EXIT_FAILURE);
		}
	}
}

/*Allocates a t_token node depending on the symbol: INPUT for '<', HERE_DOC for
'<<', OUTPUT for '>' and APPEND for '>>'. Returns a pointer to the first
character after the symbol*/
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

/*Analyzes the character located after the dollar that begins the string 'str'.
Allocates a t_token EXIT_STATUS node if the symbol is $? and a node for an
ENV_VAR if not. Returns a pointer to the first character after the symbol or
after the environment variable*/
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

/*Creates a new node in the tokens list for the first one or two symbol(s)
contained in the string 'str'. Returns 'NULL' on error or if the creation of the
node fails and a pointer to the first character after the symbol on success.
Supported symbols are listed in the SUPPORTED_SYMBOLS constant in
minishell.h*/
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
