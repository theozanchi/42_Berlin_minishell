/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper_export_builtin.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 15:06:49 by tzanchi           #+#    #+#             */
/*   Updated: 2023/12/04 14:55:34 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Get the last argument object inside the node
 * 
 * @param node The current node
 * @return Pointer to the last argument of the command node
 */
t_list	*get_current_argument(t_commands *node)
{
	t_list	*ptr;

	ptr = node->arguments;
	if (!ptr)
		return (ptr);
	while (ptr->next)
		ptr = ptr->next;
	return (ptr);
}

/**
 * @brief Get the position of the equal sign in the last argument of the command
 * node
 * 
 * @param node The current command node
 * @return Pointer to the first equal sign of the last arguments->value
 */
char	*get_equal_sign_pos(t_commands *node)
{
	t_list	*ptr;

	ptr = node->arguments;
	while (ptr->next)
		ptr = ptr->next;
	return (ft_strchr(ptr->value, '='));
}

/**
 * @brief Concatenated the current arguments->value and the new token value until
 * the closing quote is found
 * 
 * @param node The current command node
 * @param token The current token
 * @param quote_sign The sign used to quote the variable (" or ')
 * @param add_new_node A flag to indicate wether a new arg node should be
 * initialised
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	helper_quote_var(t_commands *node, t_token *token, char quote_sign,
	int *add_new_node)
{
	t_list	*current_arg;
	char	*tmp;

	current_arg = get_current_argument(node);
	tmp = ft_concat(3, current_arg->value, " ", token->value);
	if (!tmp)
		return (EXIT_FAILURE);
	free(current_arg->value);
	current_arg->value = tmp;
	if (tmp[ft_strlen(tmp) - 1] == quote_sign)
		*add_new_node = 1;
	return (EXIT_SUCCESS);
}

/**
 * @brief For the export function arguments, if the variable value is unquoted it
 * is stored in a new t_list arguments node. For a quoted value, all tokens are
 * appended in the current arguments->value string until the closing quote
 * character is found
 * 
 * @param node The current command node
 * @param token The token that is being processed
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	helper_export_builtin(t_commands *node, t_token *token)
{
	char		*equal_pos;
	static int	add_new_node = 0;

	if (!node->arguments || add_new_node)
	{
		add_new_list_node(&node->arguments, token);
		equal_pos = get_equal_sign_pos(node);
		if (equal_pos && (equal_pos + 1)
			&& (*(equal_pos + 1) == '\'' || *(equal_pos + 1) == '\"')
			&& equal_pos[ft_strlen(equal_pos) - 1] == *(equal_pos + 1))
			add_new_node = 1;
	}
	else
	{
		equal_pos = get_equal_sign_pos(node);
		if (equal_pos && (equal_pos + 1)
			&& (*(equal_pos + 1) == '\'' || *(equal_pos + 1) == '\"'))
			helper_quote_var(node, token, *(equal_pos + 1), &add_new_node);
		else
		{
			add_new_list_node(&node->arguments, token);
			add_new_node = 0;
		}
	}
	return (EXIT_SUCCESS);
}
