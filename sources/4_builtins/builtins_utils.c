/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 09:33:25 by tzanchi           #+#    #+#             */
/*   Updated: 2023/12/01 15:41:18 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Reads the command value in a commands node and returns 1 if it is one
 * of the program builtins
 * 
 * @param node The current command node
 * @return 1 if the command is a builtin, 0 if not
 */
int	cmd_is_a_builtin(t_commands *node)
{
	if (!ft_strcmp(node->command, "echo")
		|| !ft_strcmp(node->command, "cd")
		|| !ft_strcmp(node->command, "pwd")
		|| !ft_strcmp(node->command, "export")
		|| !ft_strcmp(node->command, "unset")
		|| !ft_strcmp(node->command, "env")
		|| !ft_strcmp(node->command, "exit"))
		return (1);
	else
		return (0);
}

/**
 * @brief If a command node is a builtin, launches it with correct parameters
 * 
 * @param c The command node to execute
 * @param data The main data structure
 * @param fd_in The input file descriptor
 * @param fd_out The output file descriptor
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	launch_builtin(t_commands *node, t_data *data)
{
	int	exit_code;

	if (!ft_strcmp(node->command, "echo"))
		exit_code = builtin_echo(node);
	else if (!ft_strcmp(node->command, "cd"))
		exit_code = builtin_cd(node, data);
	else if (!ft_strcmp(node->command, "pwd"))
		exit_code = builtin_pwd(node, data);
	else if (!ft_strcmp(node->command, "export"))
		exit_code = builtin_export(node, data);
	else if (!ft_strcmp(node->command, "unset"))
		exit_code = builtin_unset(node, data);
	else if (!ft_strcmp(node->command, "env"))
		exit_code = builtin_env(node, data);
	else
		exit_code = builtin_exit(node, data);
	return (exit_code);
}

/**
 * @brief If an environment variable is already in the env array, it is
 * overwritten by the new value
 * 
 * @param old The old env variable
 * @param new The new env variable
 */
int	overwrite_env_variable(char *id, char *value, t_data *data)
{
	size_t	i;
	size_t	id_length;

	i = 0;
	id_length = ft_strlen(id);
	while (ft_strncmp(data->env[i], id, id_length))
		i++;
	free(data->env[i]);
	data->env[i] = ft_concat(2, id, value);
	if (!data->env[i])
	{
		perror("minishell: overwrite_env_variable: malloc");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Allocates memory for a bigger env array, and adds the new variable at
 * the end of it
 * 
 * @param identifier The identifier of the variable
 * @param str The new value to store in env
 * @param data The main data structure
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	add_variable_to_env(char *id, char *value, t_data *data)
{
	size_t	length;
	char	**new;
	size_t	i;

	length = 0;
	while (data->env[length])
		length++;
	new = malloc((length + 2) * sizeof(char *));
	if (!new)
		return (EXIT_FAILURE);
	i = 0;
	while (i < length)
	{
		new[i] = ft_strdup(data->env[i]);
		if (!new[i])
			return (reverse_free_char_array(new, i, EXIT_FAILURE));
		i++;
	}
	new[length] = ft_concat(3, id, "=", value);
	if (!new[length])
		return (EXIT_FAILURE);
	new[length + 1] = NULL;
	free_char_array(data->env);
	data->env = new;
	return (EXIT_SUCCESS);
}
