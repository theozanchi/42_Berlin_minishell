/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 08:59:15 by tzanchi           #+#    #+#             */
/*   Updated: 2023/10/29 12:22:12 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Calls the function perror() with argument 'str'
 * 
 * @param str Error message to print
 * @return EXIT_FAILURE
 */
int	perror_return_failure(char *str)
{
	perror(str);
	return (EXIT_FAILURE);
}

/**
 * @brief Calls the function ft_printf() with argument 'str' and returns
'exit_code'
 * 
 * @param str Message to print
 * @param exit_code EXIT_SUCCESS or EXIT_FAILURE
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	ft_printf_exit_code(char *str, int exit_code)
{
	ft_printf(str);
	return (exit_code);
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
