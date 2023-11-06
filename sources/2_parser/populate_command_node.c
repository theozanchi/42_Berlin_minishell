/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   populate_command_node.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 11:43:17 by tzanchi           #+#    #+#             */
/*   Updated: 2023/10/31 16:04:07 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	populate_node_command(t_commands *node, t_token *token)
{
	if (!node)
		return (EXIT_FAILURE);
	node->command = ft_strdup(token->value);
	if (!node->command)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	populate_node_flag(t_commands *node, t_token *token)
{
	if (!node)
		return (EXIT_FAILURE);
}

int	populate_node_argument(t_commands *node, t_token *token)
{
	if (!node)
		return (EXIT_FAILURE);
}
