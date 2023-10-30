/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 15:54:25 by tzanchi           #+#    #+#             */
/*   Updated: 2023/10/30 21:12:12 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parser_helper_operands(t_data *data, t_token *token)
{
}

/**
 * @brief Creates a node in the input or output lists in the main data structure
 * 
 * @param data Main data structure
 * @param token The token containing the redirection and path of it (for <, >
 * and >>) or the delimiter for a heredoc
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	parser_helper_redirections(t_data *data, t_token *token)
{
	if (token->type == INPUT || token->type == HERE_DOC)
	{
		if (add_new_input_node(data, token))
			return (EXIT_FAILURE);
	}
	else
	{
		if (add_new_output_node(data, token))
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Initializes data->input with the standard input and data->output with
 * the standard output
 * 
 * @param data Main data structure
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	input_output_lists_init(t_data *data)
{
	{
		data->input = malloc(sizeof(t_input));
		if (!data->input)
			return (EXIT_FAILURE);
		data->input->type = STDIN;
		data->input->path = NULL;
		data->input->next = NULL;
	}
	{
		data->output = malloc(sizeof(t_output));
		if (!data->output)
			return (EXIT_FAILURE);
		data->output->type = STDOUT;
		data->output->path = NULL;
		data->output->next = NULL;
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

	if (input_output_lists_init(data))
		return (EXIT_FAILURE);
	ptr = data->tokens;
	while (ptr)
	{
		if (ptr->type <= DBL_QUOTE || ptr->type >= EXIT_CODE)
			parser_helper_operands(data, ptr);
		else if (ptr->type >= INPUT && ptr->type <= HERE_DOC)
			parser_helper_redirections(data, ptr);
		ptr = ptr->next;
	}
	return (EXIT_SUCCESS);
}
