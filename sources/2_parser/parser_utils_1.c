/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 17:02:24 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/06 16:53:17 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Loops through the input list data->input and adds a new node at the
 * end of the list
 * 
 * @param data Main data structure
 * @param new New node to add at the end of the list
 */
void	ft_inputlst_addback(t_data *data, t_input *new)
{
	t_input	*ptr;

	{
		ptr = data->input;
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = new;
	}
}

/**
 * @brief If the input list begins by a STDIN node, it is replaces by a new node
 * with data from the token passed as an argument. If the list starts with any
 * other node, the new node is added at the end of the list
 * 
 * @param data Main data structure
 * @param token Token for which a node must be created
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	add_new_input_node(t_data *data, t_token *token)
{
	t_input	*new;

	if (data->input->type == STDIN)
	{
		data->input->type = token->type;
		data->input->path = ft_strdup(token->value);
		if (!data->input->path)
			return (perror_return_failure("ft_strdup in new input node"));
	}
	else
	{
		new = malloc(sizeof(t_input));
		if (!new)
			return (perror_return_failure("new input node malloc"));
		ft_memset(new, 0, sizeof(t_input));
		new->type = token->type;
		new->path = ft_strdup(token->value);
		if (!new->path)
			return (perror_return_failure("ft_strdup in new input node"));
		ft_inputlst_addback(data, new);
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Loops through the output list data->output and adds a new node at the
end of the list
 * 
 * @param data Main data structure
 * @param new New node to add at the end of the list
 */
void	ft_outputlst_addback(t_data *data, t_output *new)
{
	t_output	*ptr;

	{
		ptr = data->output;
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = new;
	}
}

/**
 * @brief If the output list begins by a STDOUT node, it is replaces by a new
node with data from the token passed as an argument. If the list starts with any
other node, the new node is added at the end of the list
 * 
 * @param data Main data structure
 * @param token Token for which a node must be created
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	add_new_output_node(t_data *data, t_token *token)
{
	t_output	*new;

	if (data->output->type == STDOUT)
	{
		data->output->type = token->type;
		data->output->path = ft_strdup(token->value);
		if (!data->output->path)
			return (perror_return_failure("ft_strdup in new output node"));
	}
	else
	{
		new = malloc(sizeof(t_output));
		if (!new)
			return (perror_return_failure("new output node malloc"));
		ft_memset(new, 0, sizeof(t_output));
		new->type = token->type;
		new->path = ft_strdup(token->value);
		if (!new->path)
			return (perror_return_failure("ft_strdup in new output node"));
		ft_outputlst_addback(data, new);
	}
	return (EXIT_SUCCESS);
}
