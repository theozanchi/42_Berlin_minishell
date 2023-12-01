/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_executer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschott <jschott@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:46:03 by jschott           #+#    #+#             */
/*   Updated: 2023/12/01 11:32:25 by jschott          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define CEXEC_EXIT_ERR_PATH "minishell: PATH not found in env"
#define CEXEC_EXIT_ERR_CMND "minishell: command not found: "

/**
 * @brief Splits all paths returned from env into seperate strings 
 * 
 * @param env array of strings that contains the output of the command env
 * @param path_split array of strings of all paths available in env
 * @return string of the path to the executable, if it exists and user has 
 rights to execute it. otherwise NULL.
 */
char	**env_extract_paths(char **env)
{
	char	**path_split;
	char	**path_split_full;
	int		i;

	i = 0;
	while (env[i] && !ft_strnstr(env[i], "PATH", 4))
		i++;
	if (!env[i] || !(ft_strchr(env[i], '=') + 1))
		return (NULL);
	path_split = ft_split(ft_strchr(env[i], '=') + 1, ':');
	i = 0;
	while (path_split[i])
		i++;
	path_split_full = (char **)malloc((i + 1) * sizeof(char *));
	if (path_split_full)
	{
		path_split_full[i] = 0;
		while (--i >= 0)
			path_split_full[i] = ft_strjoin(path_split[i], "/");
	}
	free_char_array(path_split);
	return (path_split_full);
}

/**
 * @brief searches env for command and returns exact path if it is found
 * @param cmd command to look for
 * @param env environmental variable
 * @return string of the path to exectuable/command, NULL if it does not exist
 */
char	*search_cmd_path(t_commands *cmd, char **env)
{
	char	**paths;
	char	*exec_path;
	int		i;

	if (!cmd || !env)
		return (0);
	i = -1;
	paths = env_extract_paths(env);
	while (paths && paths[++i])
	{
		exec_path = ft_strjoin(paths[i], cmd->command);
		if (access(exec_path, X_OK | F_OK) == 0)
		{
			free_char_array (paths);
			return (exec_path);
		}
		free (exec_path);
	}
	free_char_array (paths);
	ft_putstr_fd(CEXEC_EXIT_ERR_CMND, 2);
	ft_putendl_fd(cmd->command, 2);
	return (NULL);
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
int	command_executer(t_commands *cmd, t_data *data)
{
	char	*exec_path;
	char	**paths;

	if (!cmd || !cmd->command)
		exit (EXIT_FAILURE);
	paths = 0;
	exec_path = 0;
	if (access(cmd->command, X_OK | F_OK) == 0)
		data->wstatus = execve(cmd->command, cmd->final, data->env);
	else
		exec_path = search_cmd_path(cmd, data->env);
	if (exec_path)
		data->wstatus = execve(exec_path, cmd->final, data->env);
	else
		data->wstatus = 127;
	free (exec_path);
	exit (data->wstatus);
}
