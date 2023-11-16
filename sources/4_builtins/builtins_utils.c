/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 09:33:25 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/16 12:05:21 by tzanchi          ###   ########.fr       */
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
		exit_code = builtin_pwd(node);
	else if (!ft_strcmp(node->command, "export"))
		exit_code = builtin_export();//
	else if (!ft_strcmp(node->command, "unset"))
		exit_code = builtin_unset();//
	else if (!ft_strcmp(node->command, "env"))
		exit_code = builtin_env(node->command, data);
	else
		exit_code = builtin_exit(node, data);
	return (exit_code);
}
