/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 16:22:51 by tzanchi           #+#    #+#             */
/*   Updated: 2023/10/30 20:34:51 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Allocates memory inside the t_data structure to store the environment
information from main() and copies all the information in data->env char array
 * 
 * @param data Main data structure of type t_data
 * @param env Array of char* containing environment info
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
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
			return (EXIT_FAILURE);
		}
		i++;
	}
	env[i] = NULL;
	return (EXIT_SUCCESS);
}

/**
 * @brief Extracts the PATH variable from the env char array stored in t_data 
structure and stores it in data->path

 * @param data Main data structure of type t_data
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
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
				return (EXIT_FAILURE);
			}
			return (EXIT_SUCCESS);
		}
		i++;
	}
	perror(NO_PATH);
	return (EXIT_FAILURE);
}

/**
 * @brief Extracts PATH from env variable and stores it in main data strcuture
 * 
 * @param data Main data structure
 * @param env Env variable array
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	init_data(t_data *data, char **env)
{
	if (init_env(data, env) || init_path(data))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
