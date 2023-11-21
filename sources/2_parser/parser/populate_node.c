/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   populate_node.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 11:43:17 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/21 16:15:18 by tzanchi          ###   ########.fr       */
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

/**
 * @brief Add the token value to the current command node in the arguments field.
 * If the current node uses the 'export' command, a dedicated parsing function
 * helper_export_builtin() is called
 * 
 * @param node The current node
 * @param token The current token
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	populate_node_argument(t_commands *node, t_token *token)
{
	if (!node)
		return (EXIT_FAILURE);
	if (ft_strcmp(node->command, "export"))
	{
		if (add_new_list_node(&node->arguments, token))
			return (EXIT_FAILURE);
	}
	else
		helper_export_builtin(node, token);
	return (EXIT_SUCCESS);
}
