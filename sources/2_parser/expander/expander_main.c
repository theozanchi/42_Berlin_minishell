/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 15:54:22 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/13 20:52:36 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Loop through the string str to expand all the variables it contains
 * 
 * @param str Double pointer  to the beginning of the string
 * @param data The main data structure to access the environment variable
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	expand_string(char **str, t_data *data)
{
	size_t	i;
	char	*tmp;

	i = 0;
	while ((*str)[i])
	{
		if ((*str)[i] == '$')
		{
			tmp = concatenate_expanded_string(str, &i, data);
			if (!tmp)
				return (EXIT_FAILURE);
			free(*str);
			*str = tmp;
		}
		else
			i++;
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Expands the command string if it is not single quoted
 * 
 * @param node The current command node
 * @param data The main data structure
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	expand_command(t_commands *node, t_data *data)
{
	if (node->cmd_quote != SGL)
	{
		if (expand_string(&node->command, data))
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Expands all the non quoted arguments comntained in a t_list object
 * 
 * @param list Pointer to the list to expand
 * @param data The main data structure
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	expand_list_of_str(t_list *list, t_data *data)
{
	while (list)
	{
		if (list->quote != SGL)
		{
			if (expand_string(&list->value, data))
				return (EXIT_FAILURE);
		}
		list = list->next;
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Loop through all commands, their arguments and their flags to expand
 * all tokens that are not single-quoted
 * 
 * @param data The main data structure
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	expander(t_data *data)
{
	t_commands	*cmd_node;

	cmd_node = data->commands;
	while (cmd_node)
	{
		if (expand_command(cmd_node, data))
			return (EXIT_FAILURE);
		if (expand_list_of_str(cmd_node->arguments, data))
			return (EXIT_FAILURE);
		if (expand_list_of_str(cmd_node->flags, data))
			return (EXIT_FAILURE);
		cmd_node = cmd_node->next;
	}
	return (EXIT_SUCCESS);
}
