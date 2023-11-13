/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 15:54:25 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/13 18:38:03 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief If a token is an operand, it is analyzed and stored properly depending
 * on whether it is a command, a flag or an argument. If the token before an
 * NO_QUOTE is an operator, then a new command node is created and added at the
 * end of the commands list thanks to a static variable
 * 
 * @param data The main data structure
 * @param token The current token being parsed
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	parser_helper_operands(t_data *data, t_token *token)
{
	static t_commands	*ptr = NULL;

	if (!token->prev
		|| (token->prev->type >= PIPE && token->prev->type <= HERE_DOC))
	{
		ptr = add_new_command_node(data);
		if (populate_node_command(ptr, token))
			return (EXIT_FAILURE);
	}
	else if (token->value[0] == '-')
	{
		if (populate_node_flag(ptr, token))
			return (EXIT_FAILURE);
	}
	else
	{
		if (populate_node_argument(ptr, token))
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Opens a file for input or output redirection. If a previous file had
 * been opened, it is first closed
 * 
 * @param redirection The input or output structure to modify 
 * @param token The current token
 * @param oflag The flags for opneing the file
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	open_redirection_fd(t_io *redirection, t_token *token, int oflag)
{
	if (redirection->type != STDIN && redirection->type != STDOUT)
		close(redirection->fd);
	redirection->type = token->type;
	redirection->value = ft_strdup(token->value);
	if (!redirection->value)
		return (perror_return_failure("ft_strdup for redirection value"));
	redirection->fd = open(token->value, oflag, 0644);
	if (redirection->fd < 0)
		return (perror_return_failure(redirection->value));
	return (EXIT_SUCCESS);
}

/**
 * @brief Modifies the input and output values in the main data structure
 * 
 * @param data The main data structure
 * @param token The current token
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	parser_helper_redirections(t_data *data, t_token *token)
{
	if (token->type == INPUT || token->type == HERE_DOC)
	{
		if (open_redirection_fd(&data->input, token, O_RDONLY))
			return (EXIT_FAILURE);
	}
	else if (token->type == OUTPUT)
	{
		if (open_redirection_fd(&data->output, token, O_WRONLY | O_CREAT))
			return (EXIT_FAILURE);
	}
	else
	{
		if (open_redirection_fd(&data->output, token,
				O_WRONLY | O_APPEND | O_CREAT))
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Loops through the list of tokens to create three lists: the list of
 * commands to execute, the list of all INPUT and HERE_DOC and the list of
 * OUTPUT and APPEND
 * 
 * @param data Main data structure
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	parser(t_data *data)
{
	t_token	*ptr;

	ptr = data->tokens;
	while (ptr)
	{
		if (ptr->type <= DBL_QUOTE)
			parser_helper_operands(data, ptr);
		else if (ptr->type >= INPUT && ptr->type <= HERE_DOC)
			parser_helper_redirections(data, ptr);
		ptr = ptr->next;
	}
	if (expander(data))
		return (EXIT_FAILURE);
	if (concatenate_successive_commands(data))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
