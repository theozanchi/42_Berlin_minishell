/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 08:59:15 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/07 18:03:55 by tzanchi          ###   ########.fr       */
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
 * @brief Loops through a t_list list and adds a new node at the end
 * 
 * @param list The list to add a new node to
 * @param new The new node to add
 */
void	ft_lst_addback(t_list *list, t_list *new)
{
	t_list	*ptr;

	{
		ptr = list;
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = new;
	}
}

/**
 * @brief Creates and adds a new node at the end of of a list, with str as a
 * node value
 * 
 * @param list The list in which to create the new node
 * @param str The string that will be the value of the new node
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	add_new_list_node(t_list **list, char *str)
{
	t_list	*new;

	new = malloc(sizeof(t_output));
	if (!new)
		return (perror_return_failure("new list node malloc"));
	ft_memset(new, 0, sizeof(t_list));
	new->value = ft_strdup(str);
	if (!new->value)
		return (perror_return_failure("ft_strdup in add_new_list_node"));
	if (!*list)
		*list = new;
	else
		ft_lst_addback(*list, new);
	return (EXIT_SUCCESS);
}
