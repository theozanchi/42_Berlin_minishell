/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 11:28:35 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/06 15:56:19 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_commandlst_addback(t_data *data, t_commands *new)
{
	t_commands	*ptr;

	{
		ptr = data->commands;
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = new;
	}
}

t_commands	*add_new_command_node(t_data *data)
{
	t_commands	*new;

	if (!data->commands)
	{
		data->commands = malloc(sizeof(t_commands));
		if (!data->commands)
		{
			perror("new command node malloc");
			return (NULL);
		}
		ft_memset(data->commands, 0, sizeof(t_commands));
		return (data->commands);
	}
	else
	{
		new = malloc(sizeof(t_commands));
		if (!new)
		{
			perror("new command node malloc");
			return (NULL);
		}
		ft_memset(new, 0, sizeof(t_commands));
		ft_commandlst_addback(data, new);
		return (new);
	}
}
