/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 16:22:51 by tzanchi           #+#    #+#             */
/*   Updated: 2023/10/05 12:04:13 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*Allocates memory inside the t_data structure to store the environment
information from main() and copies all the information in data->env char array
Returns -1 in case of failure, 0 in case of success*/
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

/*Extracts the PATH variable from the env char array stored in t_data strcuture
and stores it in data->path
Returns -1 in case of failure, 0 in case of success*/
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

/*Initializes the data structure:
• Sets data->env with the environment information (char **)
• Extract the PATH from env and stores it in data->path (char *)
• Initializes the history to NULL (t_history)*/
int	init_data(t_data *data, char **env)
{
	if (init_env(data, env) || init_path(data))
		return (EXIT_FAILURE);
	data->history = NULL;
	return (EXIT_SUCCESS);
}

void	launch_minishell(t_data *data)
{
	t_token	*ptr;

	while (1)
	{
		data->argv = readline(ENTRY_PROMPT);
		add_history(data->argv);
		lexer(data);
		ptr = data->tokens;
		while (ptr)
		{
			printf("%s\n", ptr->raw_command);
			ptr = ptr->next;
		}
		// parser(data);
		// executer(data);
		free(data->argv);
		data->argv = NULL;
	}
}
