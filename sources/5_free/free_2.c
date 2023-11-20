/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 20:29:08 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/20 15:15:43 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Frees all the data contained in `data` and sets every element to NULL
 * 
 * @param data Main data structure of type t_data
 */
void	free_all_memory(t_data *data)
{
	if (!data)
		return ;
	free_char_array(data->env);
	if (data->argv)
	{
		free(data->argv);
		data->argv = NULL;
	}
	if (data->path)
	{
		free(data->path);
		data->path = NULL;
	}
	free_tokens(data);
	free_commands(data);
	free_and_reset_io(data);
}

/**
 * @brief Frees memory allocated for argv, tokens, inputs, commands and outputs
 * and sets all pointers to NULL
 * 
 * @param data Main data structure
 */
void	free_memory_between_commands(t_data *data)
{
	if (data->argv)
	{
		free(data->argv);
		data->argv = NULL;
	}
	free_tokens(data);
	free_commands(data);
	free_and_reset_io(data);
}

/**
 * @brief Frees a list of type t_list and sets all pointers to NULL
 * 
 * @param list The list to free
 */
void	free_list(t_list *list)
{
	t_list	*tmp;

	while (list != NULL)
	{
		tmp = list;
		list = list->next;
		tmp->next = NULL;
		if (tmp->value)
		{
			free(tmp->value);
			tmp->value = NULL;
		}
		free(tmp);
		tmp = NULL;
	}
}

/**
 * @brief Frees and resets the input and output values to set them to standard
 * input and output
 * 
 * @param data The main data structure
 */
void	free_and_reset_io(t_data *data)
{
	data->input.type = STDIN;
	if (data->input.value)
	{
		free(data->input.value);
		data->input.value = NULL;
	}
	data->input.fd = 0;
	data->output.type = STDOUT;
	if (data->output.value)
	{
		free(data->output.value);
		data->output.value = NULL;
	}
	data->output.fd = 1;
}

int	reverse_free_char_array(char **array, size_t i, int exit_code)
{
	while (i)
	{
		free(array[i]);
		array[i] = NULL;
		i--;
	}
	array = NULL;
	return (exit_code);
}
