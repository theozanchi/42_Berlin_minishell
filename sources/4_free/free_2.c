/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 20:29:08 by tzanchi           #+#    #+#             */
/*   Updated: 2023/10/30 21:11:31 by tzanchi          ###   ########.fr       */
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
	free_input(data);
	free_output(data);
	free_commands(data);
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
	free_input(data);
	free_commands(data);
	free_output(data);
}
