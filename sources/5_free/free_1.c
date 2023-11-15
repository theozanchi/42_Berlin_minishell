/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 20:17:48 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/08 13:02:36 by tzanchi          ###   ########.fr       */
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
		if (tmp->arguments)
			free_list(tmp->arguments);
		if (tmp->flags)
			free_list(tmp->flags);
		free_char_array(tmp->final);
		free(tmp);
		tmp = NULL;
	}
}
