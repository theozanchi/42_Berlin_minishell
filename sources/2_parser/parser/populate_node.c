/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   populate_node.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 11:43:17 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/20 21:14:58 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Adds the token value to the current command node in the command field
 * 
 * @param node The current node
 * @param token The current token
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	populate_node_command(t_commands *node, t_token *token)
{
	if (!node)
		return (EXIT_FAILURE);
	node->command = ft_strdup(token->value);
	if (!node->command)
		return (EXIT_FAILURE);
	node->cmd_quote = token->quote;
	return (EXIT_SUCCESS);
}

/**
 * @brief Add the token value to the current command node in the flags field
 * 
 * @param node The current node
 * @param token The current token
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	populate_node_flag(t_commands *node, t_token *token)
{
	if (!node)
		return (EXIT_FAILURE);
	if (add_new_list_node(&node->flags, token))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static char	*get_equal_sign_pos(t_commands *node)
{
	t_list	*ptr;

	ptr = node->arguments;
	while (ptr->next)
		ptr = ptr->next;
	return (ft_strchr(ptr->value, '='));
}

/**
 * @brief Add the token value to the current command node in the arguments field
 * 
 * @param node The current node
 * @param token The current token
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	populate_node_argument(t_commands *node, t_token *token)
{
	char	*tmp;

	if (!node)
		return (EXIT_FAILURE);
	if (ft_strcmp(node->command, "export"))
	{
		if (add_new_list_node(&node->arguments, token))
			return (EXIT_FAILURE);
	}
	else
	{
		if (!node->arguments)
			add_new_list_node(&node->arguments, token);
		else
		{
			char	*equal_pos = get_equal_sign_pos(node);
			if (equal_pos
				&& (*(equal_pos + 1) == '\'' || *(equal_pos + 1) == '\"'))
			{
				t_list	*ptr = node->arguments;
				while (ptr->next)
					ptr = ptr->next;
				tmp = ft_concat(3, ptr->value, " ", token->value);
				if (!tmp)
					return (EXIT_FAILURE);
				free(ptr->value);
				ptr->value = tmp;
			}
			else
				add_new_list_node(&node->arguments, token);
		}
	}
	return (EXIT_SUCCESS);
}
