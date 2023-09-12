/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 16:22:51 by tzanchi           #+#    #+#             */
/*   Updated: 2023/09/12 18:25:05 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_env(t_data *data, char **env)
{
	int	i;

	i = 0;
	data->env = ft_calloc(get_char_arr_length(env) + 1, sizeof(char *));
	while (env[i])
	{
		data->env[i] = ft_strdup(env[i]);
		if (!data->env[i])
		{
			perror(ERR_ENV_SETUP);
			return (-1);
		}
		i++;
	}
	env[i] = NULL;
	return (0);
}

int	init_path(t_data *data)
{
	int	i;

	i = 0;
	while (data->env[i])
	{
		if (!ft_strncmp(data->env[i], "PATH=", 5))
		{
			data->path = ft_strdup(data->env[i]);
			if (!data->path)
			{
				perror(ERR_PATH_SETUP);
				return (-1);
			}
			return (0);
		}
		i++;
	}
	perror(NO_PATH);
	return (-1);
}

int	init_data(t_data *data, char **env)
{
	if (init_env(data, env) == -1
		|| init_path(data))
		return (-1);
	// printf("%s\n", data->path);
	return (0);
}

void	launch_minishell(t_data *data)
{
	while (1)
	{
		data->argv = readline(ENTRY_PROMPT);
	}
}
