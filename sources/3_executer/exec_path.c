/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschott <jschott@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 16:04:45 by jschott           #+#    #+#             */
/*   Updated: 2023/10/30 16:45:18 by jschott          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	free_str_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
	return (1);
}

char	*get_exec_path(char *cmd, char **env)
{
	char	**path_split;
	int		i;
	char	*path_complete;
	char	*exec_path;

	path_split = 0;
	i = 0;
	while (env[i] && !ft_strnstr(env[i], "PATH", 4))
		i++;
	path_split = ft_split(ft_strchr(env[i], '/'), ':');
	i = -1;
	while (path_split[++i])
	{
		path_complete = ft_strjoin(path_split[i], "/");
		exec_path = ft_strjoin(path_complete, cmd);
		free (path_complete);
		if (access(exec_path, X_OK | F_OK) == 0)
		{
			free_str_arr (path_split);
			return (exec_path);
		}
		free (exec_path);
	}
	free_str_arr (path_split);
	return (0);
}
