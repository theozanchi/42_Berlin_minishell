/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 16:22:40 by tzanchi           #+#    #+#             */
/*   Updated: 2023/10/29 12:07:30 by tzanchi          ###   ########.fr       */
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
 * @brief Frees all the data contained in `data` and sets every element to NULL
 * 
 * @param data Main data structure of type t_data
 */
void	free_memory(t_data *data)
{
	if (!data)
		return ;
	free_char_array(data->env);
	free(data->argv);
	data->argv = NULL;
	free(data->path);
	data->path = NULL;
}

/**
 * @brief Frees and sets all the memory allocated for the programm to NULL and
terminates the process with exit code `exit_code`
 * 
 * @param data Main data structure of type t_data
 * @param exit_code EXIT_SUCCES or EXIT_FAILURE used to exit() the program
 */
void	exit_minishell(t_data *data, int exit_code)
{
	free_memory(data);
	rl_clear_history();
	exit(exit_code);
}
