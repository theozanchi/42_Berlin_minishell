/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 20:17:48 by tzanchi           #+#    #+#             */
/*   Updated: 2023/10/30 21:10:45 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Loops through tokens in main data structure to free each node
 * 
 * @param data Main data structure
 */
void	free_tokens(t_data *data)
{
	t_token	*tmp;

	while (data->tokens != NULL)
	{
		tmp = data->tokens;
		data->tokens = data->tokens->next;
		tmp->prev = NULL;
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
 * @brief Frees an array of char *, sets every char * to NULL and the array to
NULL
 * 
 * @param array Array of char*, env variable
 */
void	free_char_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
	array = NULL;
}

/**
 * @brief Frees the list of inputs and sets all pointers to NULL
 * 
 * @param data Main data structure
 */
void	free_input(t_data *data)
{
	t_input	*tmp;

	while (data->input != NULL)
	{
		tmp = data->input;
		data->input = data->input->next;
		tmp->next = NULL;
		if (tmp->path)
		{
			free(tmp->path);
			tmp->path = NULL;
		}
		free(tmp);
		tmp = NULL;
	}
}

/**
 * @brief Frees the list of outputs and sets all pointers to NULL
 * 
 * @param data Main data structure
 */
void	free_output(t_data *data)
{
	t_output	*tmp;

	while (data->output != NULL)
	{
		tmp = data->output;
		data->output = data->output->next;
		tmp->next = NULL;
		if (tmp->path)
		{
			free(tmp->path);
			tmp->path = NULL;
		}
		free(tmp);
		tmp = NULL;
	}
}

/**
 * @brief Frees the list of commands and sets all pointers to NULL
 * 
 * @param data Main data structure
 */
void	free_commands(t_data *data)
{
	t_commands	*tmp;

	while (data->commands != NULL)
	{
		tmp = data->commands;
		data->commands = data->commands->next;
		tmp->next = NULL;
		if (tmp->command)
		{
			free(tmp->command);
			tmp->command = NULL;
		}
		if (tmp->argument)
		{
			free(tmp->argument);
			tmp->argument = NULL;
		}
		free_char_array(tmp->flags);
		free(tmp);
		tmp = NULL;
	}
}
