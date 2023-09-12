/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 16:22:40 by tzanchi           #+#    #+#             */
/*   Updated: 2023/09/12 17:49:29 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_char_array(char **array)
{
	char	**ptr;

	if (!array || !*array)
		return ;
	ptr = array;
	while (ptr)
	{
		free(*ptr);
		*ptr = NULL;
		ptr++;
	}
	free(array);
	array = NULL;
}

void	free_memory(t_data *data)
{
	if (!data)
		return ;
	free_char_array(data->env);
	free(data->argv);
	free(data->path);
}

void	exit_minishell(t_data *data, int exit_code)
{
	free_memory(data);
	exit(exit_code);
}
