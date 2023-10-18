/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 18:55:37 by tzanchi           #+#    #+#             */
/*   Updated: 2023/10/18 18:01:12 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*Allocates memory for a new t_token node, then allocates enough memory to store
the raw command contained between the pointers start_ptr and end_ptr and copies
the raw command into new_raw_command. If a malloc() fails, NULL is returned*/
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
