/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _utils_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 08:59:15 by tzanchi           #+#    #+#             */
/*   Updated: 2023/10/03 09:32:31 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	perror_return_failure(char *str)
{
	perror(str);
	return (EXIT_FAILURE);
}

void	ft_lstaddback(t_data *data, t_token *new)
{
	t_token	*ptr;
	t_token	*prev;

	if (!data->tokens)
	{
		new->prev = NULL;
		data->tokens = new;
	}
	else
	{
		ptr = data->tokens;
		while (ptr->next)
		{
			prev = ptr;
			ptr = ptr->next;
		}
		new->prev = prev;
		ptr->next = new;
	}
}
