/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 18:55:37 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/13 20:14:36 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Creates and returns a new token from user input
 * 
 * @param start Pointer to the beginning of the token
 * @param end Pointer to the end of the token
 * @param type type of token to store
 * @return Pointer to new token
 */
t_token	*new_token(char *start, char *end, t_type type, t_quote quote)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
	{
		perror("new_node()");
		return (NULL);
	}
	if (start)
	{
		new->value = malloc((end - start + 1) * sizeof(char));
		if (!new->value)
		{
			perror("token value in new_node()");
			return (NULL);
		}
		ft_strlcpy(new->value, start, end - start + 1);
	}
	else
		new->value = NULL;
	new->type = type;
	new->quote = quote;
	new->next = NULL;
	return (new);
}

/**
 * @brief Checks if a symbol is at the end of a string
 * 
 * @param str User input
 * @return EXIT_SUCCES or EXIT_FAILURE
 */
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

/**
 * @brief Checks for two consecutive SUPPORTED_SYMBOLS. If two consecutive
symbols are found, only '<<' and '>>' are valid combinations
 * 
 * @param str User input
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
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

/**
 * @brief Loops through the token list data->tokens and adds a new node at the
end of the list
 * 
 * @param data Main data structure
 * @param new New token to add at the end of the token list
 */
void	ft_tokenlst_addback(t_data *data, t_token *new)
{
	t_token	*ptr;

	if (!data->tokens)
	{
		new->prev = NULL;
		data->tokens = new;
	}
	else
	{
		ptr = data->tokens;
		while (ptr->next)
			ptr = ptr->next;
		new->prev = ptr;
		ptr->next = new;
	}
}
