/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_extract_path.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschott <jschott@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 16:04:45 by jschott           #+#    #+#             */
/*   Updated: 2023/11/09 15:58:20 by jschott          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"

/**
 * @brief Frees a given array of strings
 * 
 * @param arr Array with pointers to strings
 * @param int index
 * @return 1 if success. 0 if arr is NULL
 */
/* int	free_str_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
	return (1);
}
 */
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

printf("HELLO extract path\n\n");
	path_split = 0;
	i = 0;
	while (env[i] && !ft_strnstr(env[i], "PATH", 4))
		i++;
	path_split = ft_split(ft_strchr(env[i], '/'), ':');
	i = 0;
	while (path_split[i])
		i++;
	path_split_full = (char **) malloc (i);
	if (path_split_full)
	{
		path_split_full[i] = 0;
		--i;
		while (--i >= 0)
			path_split_full[i] = ft_strjoin(path_split[i], "/");
	}
printf("CIAO extract path\n\n");
	free_str_arr(path_split);
	return (path_split_full);
}
