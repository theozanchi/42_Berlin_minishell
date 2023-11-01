/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_execute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschott <jschott@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:46:03 by jschott           #+#    #+#             */
/*   Updated: 2023/11/01 17:51:25 by jschott          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executer.h"

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
int	cmd_execute(t_commands *cmd, char **env)
{
	char	*exec_path;
	char	**paths;
	int		i;

	if (!cmd || !cmd->command)
		return (-1);
	i = -1;
// check if command is a direct path to existing executable
	if (access(cmd->command, X_OK | F_OK) == 0)
		exec_path = cmd->command;
	else
	{
		paths = env_extract_paths(env);
// check if command is included in builtin
		exec_path = ft_strjoin(BUILTIN_PATH, cmd->command);
// check if command is included in env
		while (access(exec_path, X_OK | F_OK) != 0 && paths[++i])
		{
			free (exec_path);
			exec_path = ft_strjoin(paths[i], cmd->command);
		}
		free (paths);
	}
	execve(exec_path, cmd->flags, env);
	if (i >= 0)
		free (exec_path);
	return (1);
}
