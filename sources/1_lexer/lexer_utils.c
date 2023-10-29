/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 18:55:37 by tzanchi           #+#    #+#             */
/*   Updated: 2023/10/29 12:21:07 by tzanchi          ###   ########.fr       */
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
t_token	*new_node(char *start, char *end, t_type type)
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
	new->next = NULL;
	return (new);
}
