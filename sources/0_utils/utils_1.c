/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 08:59:15 by tzanchi           #+#    #+#             */
/*   Updated: 2023/10/16 18:52:47 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*Calls the fucntion perror() with argument 'str' and returns 'EXIT_FAILURE'*/
int	perror_return_failure(char *str)
{
	perror(str);
	return (EXIT_FAILURE);
}

/*Calls the function ft_printf() with argument 'str' and returns
'EXIT_FAILURE'*/
int	ft_printf_exit_code(char *str, int exit_code)
{
	ft_printf(str);
	return (exit_code);
}

/*Loops through the token list data->tokens and adds a new node at the end of
the list*/
void	ft_tokenlst_addback(t_data *data, t_token *new)
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
