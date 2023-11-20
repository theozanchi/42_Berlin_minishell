/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 15:54:25 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/20 18:47:03 by tzanchi          ###   ########.fr       */
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
int	parser_helper_operands(t_data *data, t_token *token, int *create_new_node)
{
	static t_commands	*ptr = NULL;

	if (*create_new_node)
	{
		ptr = add_new_command_node(data);
		*create_new_node = 0;
		if (!ptr)
			return (EXIT_FAILURE);
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
int	open_redirection_fd(t_data *data, t_io *redir, t_token *token, int oflag)
{
	if (redir->type != STDIN && redir->type != STDOUT
		&& redir->type != HERE_DOC)
		close(redir->fd);
	redir->type = token->type;
	redir->quote = token->quote;
	redir->value = ft_strdup(token->value);
	if (!redir->value)
		return (perror_return_failure("ft_strdup for redirection value"));
	if (redir->quote != SGL)
		expand_string(&redir->value, data);
	if (redir->type != HERE_DOC)
		redir->fd = open(redir->value, oflag, 0644);
	if (redir->fd < 0)
		return (perror_return_failure(redir->value));
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
		if (open_redirection_fd(data, &data->input, token, O_RDONLY))
			return (EXIT_FAILURE);
	}
	else if (token->type == OUTPUT)
	{
		if (open_redirection_fd(data, &data->output, token, 
				O_WRONLY | O_TRUNC | O_CREAT))
			return (EXIT_FAILURE);
	}
	else
	{
		if (open_redirection_fd(data, &data->output, token,
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
	int		create_new_node;

	ptr = data->tokens;
	create_new_node = 1;
	while (ptr)
	{
		if (ptr->type == PIPE)
			create_new_node = 1;
		else if (ptr->type == OPERAND)
			parser_helper_operands(data, ptr, &create_new_node);
		else
			parser_helper_redirections(data, ptr);
		ptr = ptr->next;
	}
	if (create_new_node && *data->argv)
		return (ft_printf_exit_code("No command after pipe\n", EXIT_FAILURE));
	if (expander(data))
		return (EXIT_FAILURE);
	if (concatenate_successive_commands(data))
		return (EXIT_FAILURE);
	if (data->input.type == HERE_DOC && here_doc(data))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
