/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 21:15:09 by tzanchi           #+#    #+#             */
/*   Updated: 2023/09/12 22:51:56 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*Adds the current command sqtored in data->argv to the history of commands
stored inn data->history*/
void	add_command_to_history(t_data *data)
{
	t_history	*new_command;

	if (!data || !data->argv)
		return ;
	new_command = malloc(sizeof(t_history));
	if (!new_command)
	{
		perror(ERR_HIST);
		return ;
	}
	new_command->argv = ft_strdup(data->argv);
	new_command->previous = data->history;
	if (data->history)
		data->history->next = new_command;
	new_command->next = NULL;
	data->history = new_command;
}
