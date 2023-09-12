/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 16:22:40 by tzanchi           #+#    #+#             */
/*   Updated: 2023/09/12 22:50:37 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*Frees an array of char *, sets every char * to NULL and the array to NULL*/
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

/*Frees a list of type t_history and sets every node to NULL*/
void	free_list(t_data *data)
{
	t_history	*current;
	t_history	*next;

	current = data->history;
	while (current)
	{
		next = current->next;
		free(current->argv);
		free(current);
		current = next;
	}
	data->history = NULL;
}

/*Frees all the data contained in `data` and sets every element to NULL*/
void	free_memory(t_data *data)
{
	if (!data)
		return ;
	free_char_array(data->env);
	free(data->argv);
	data->argv = NULL;
	free(data->path);
	data->path = NULL;
	free_list(data);
}

/*Frees and sets all the memory allocated for the programm to NULL and
terminates the process with exit code `exit_code`*/
void	exit_minishell(t_data *data, int exit_code)
{
	free_memory(data);
	exit(exit_code);
}
