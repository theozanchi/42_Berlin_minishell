/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_execute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschott <jschott@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:46:03 by jschott           #+#    #+#             */
/*   Updated: 2023/11/01 16:47:52 by jschott          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executer.h"

int	cmd_execute(char **paths, t_commands *cmd, char **env)
{
	char	*exec_path;
	int		i;

	if (!cmd || !cmd->command)
		return (-1);
	i = -1;
// check if command is a direct path to existing executable
	if (access(cmd->command, X_OK | F_OK) == 0)
		exec_path = cmd->command;
	else
	{
// check if command is included in builtin
		exec_path = ft_strjoin(BUILTIN_PATH, cmd->command);
// check if command is included in env
		while (paths[++i])
		{
			if (access(exec_path, X_OK | F_OK) == 0)
				break ;
			exec_path = ft_strjoin(paths[i], cmd->command);
		}
	}
	execve(exec_path, cmd->flags, env);
	if (i >= 0)
		free (exec_path);
	return (1);
}
