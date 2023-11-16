/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_execute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:46:03 by jschott           #+#    #+#             */
/*   Updated: 2023/11/16 11:34:16 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "executer.h"
#include "minishell.h"

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

// printf("HELLO EXEC\n%s\n\n", cmd->command);
	if (!cmd || !cmd->command)
		exit (EXIT_FAILURE);
	i = 0;
	paths = 0;
// check if command is a direct path to existing executable
	if (access(cmd->command, X_OK | F_OK) == 0)
		exec_path = cmd->command;
	else
	{
		paths = env_extract_paths(env);
// check if command is included in builtin
	// if (cmd_is_a_builtin(cmd))
	// 	launch_builtin(cmd, data)
	// else launch execve
			// exec_path = ft_strjoin(BUILTIN_PATH, cmd->command);
		exec_path = ft_strjoin(paths[0], cmd->command);
		while (paths[++i] && access(exec_path, X_OK | F_OK) != 0)
		{
			free (exec_path);
			exec_path = ft_strjoin(paths[i], cmd->command);
		}
		free_char_array (paths);
	}
	if (execve(exec_path, cmd->final, env) == -1)
	{
		write(2, "COMMAND ", 8);
		write(2, cmd->command, ft_strlen(cmd->command));
		write(2, " NOT FOUND\n", 11);
		free (exec_path);
		exit (EXIT_FAILURE);
	}
	free (exec_path);
	exit (EXIT_SUCCESS);
}
