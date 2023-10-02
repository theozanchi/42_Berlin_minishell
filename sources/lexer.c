/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 11:49:34 by tzanchi           #+#    #+#             */
/*   Updated: 2023/10/02 18:59:51 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_node(char *arg)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
	{
		perror("Token malloc");
		return (NULL);
	}
	new->raw_command = ft_strdup(arg);
	new->next = NULL;
}

void	save_non_quoted_arg(t_data *data, char *arg)
{
	if (!data->tokens)
	{
		data->tokens = new_node(arg);
		data->tokens->prev = NULL;
	}
	else
		ft_lstaddback(new_node(arg));
}

void	lexer(t_data *data)
{
	char	*split;
	char	*ptr;

	split = ft_split(data->argv, " ");
	ptr = split;
	data->tokens = malloc(sizeof(t_token));
	data->tokens = NULL;
	while (ptr)
	{
		if (*ptr == '\'' || *ptr == '\"')
			save_quoted_arg(data, &ptr);
		else
			save_non_quoted_arg(data, *ptr++);
	}
	free(split);
}
