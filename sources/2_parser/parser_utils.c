/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 11:28:35 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/08 12:59:54 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Loops through the commands list data->commands and adds a new node at
 * the end of the list
 * 
 * @param data Main data structure
 * @param new New node to add at the end of the list
 */
void	ft_commandlst_addback(t_data *data, t_commands *new)
{
	t_commands	*ptr;

	{
		ptr = data->commands;
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = new;
	}
}

/**
 * @brief Creates and adds at the end of the command list of the main data
 * structure. The memory is set to zero
 * 
 * @param data The main data structure
 * @return A pointer to the newly created node
 */
t_commands	*add_new_command_node(t_data *data)
{
	t_commands	*new;

	if (!data->commands)
	{
		data->commands = malloc(sizeof(t_commands));
		if (!data->commands)
		{
			perror("new command node malloc");
			return (NULL);
		}
		ft_memset(data->commands, 0, sizeof(t_commands));
		return (data->commands);
	}
	else
	{
		new = malloc(sizeof(t_commands));
		if (!new)
		{
			perror("new command node malloc");
			return (NULL);
		}
		ft_memset(new, 0, sizeof(t_commands));
		ft_commandlst_addback(data, new);
		return (new);
	}
}

/**
 * @brief Get the number of elements (command + arguments + flags) in a command
 * node
 * 
 * @param node The current t_command node
 * @return The number of elements, size_t
 */
size_t	get_number_of_command_elements(t_commands *node)
{
	size_t	count;
	t_list	*arg_ptr;
	t_list	*flag_ptr;

	count = 1;
	arg_ptr = node->arguments;
	flag_ptr = node->flags;
	while (node)
	{
		while (arg_ptr)
		{
			count++;
			arg_ptr = arg_ptr->next;
		}
		while (flag_ptr)
		{
			count++;
			flag_ptr = flag_ptr->next;
		}
		node = node->next;
	}
	return (count);
}
