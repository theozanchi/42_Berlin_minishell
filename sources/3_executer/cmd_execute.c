/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_execute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschott <jschott@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:46:03 by jschott           #+#    #+#             */
/*   Updated: 2023/11/16 15:40:33 by jschott          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "executer.h"
#include "minishell.h"

/// @brief searches env for command and returns exact path if it is found
/// @param cmd command to look for
/// @param env environmental variable
/// @return string of the path to exectuable/command, NULL if it does not exist
char	*search_cmd_path(t_commands *cmd, char **env)
{
	char	**paths;
	char	*exec_path;
	int		i;

	if (!cmd || !env)
		return (0);
	i = -1;
	paths = env_extract_paths(env);
	exec_path = ft_strjoin(paths[0], cmd->command);
	while (paths[++i] && access(exec_path, X_OK | F_OK) != 0)
	{
		free (exec_path);
		exec_path = ft_strjoin(paths[i], cmd->command);
	}
	free_char_array (paths);
	return (exec_path);
}

/**
 * @brief checks if a given path is a direct path, a builtin or part of env.
executes command if it exists
 * 		 
 * 
 * @param paths array of strings of all paths available in env 
 * @param cmd command incl. arguments and flags to execute
 * @param env environmental variable
 * @param exec_path path to potential executable, built with paths & BUILTIN_PATH
 * @return string of the path to the executable, if it exists and user has 
 rights to execute it. otherwise NULL.
 */
int	cmd_execute(t_commands *cmd, t_data *data)
{
	char	*exec_path;
	char	**paths;

	if (!cmd || !cmd->command)
		exit (EXIT_FAILURE);
	paths = 0;
	if (cmd_is_a_builtin(cmd))
		launch_builtin(cmd, data);
	if (access(cmd->command, X_OK | F_OK) == 0)
		exec_path = cmd->command;
	else
		exec_path = search_cmd_path(cmd, data->env);
	if (!exec_path)
		return (EXIT_FAILURE);
	data->wstatus = execve(exec_path, cmd->final, data->env) == -1;
	if (data->wstatus == -1)
	{
		perror("command not found: ");
		perror(cmd->command);
		free (exec_path);
		return (EXIT_FAILURE);
	}
	free (exec_path);
	return (EXIT_SUCCESS);
}
