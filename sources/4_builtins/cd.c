/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 18:17:46 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/16 15:25:43 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define CD_ERR_HOME_NOT_FOUND "minishell: cd: HOME directory not found in env\n"
#define CD_ERR_PWD_NOT_FOUND "minishell: cd: PWD directory not found in env\n"
#define CD_ERR_FLAGS "minishell: cd: no options supported\n"
#define CD_ERR_EXTRA_ARG "minishell: cd: too many arguments\n"

/**
 * @brief Loops through the current env variable to locate the HOME path
 * 
 * @param data The main data structure
 * @return Pointer to the path or NULL on error
 */
static char	*get_home_directory(t_data *data)
{
	size_t	i;

	i = 0;
	while (data->env[i])
	{
		if (!ft_strncmp(data->env[i], "HOME=", 5))
			return (data->env[i] + 5);
		i++;
	}
	ft_printf(CD_ERR_HOME_NOT_FOUND);
	return (NULL);
}

/**
 * @brief Replaces the PWD value in the current env variable by its updated
 * version
 * 
 * @param new_directory The new working directory
 * @param data The main data structure
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int	update_env_variable(char *new_directory, t_data *data)
{
	size_t	i;

	i = 0;
	while (data->env[i])
	{
		if (!ft_strncmp(data->env[i], "PWD=", 4))
		{
			free(data->env[i]);
			data->env[i] = ft_concat(2, "PWD=", new_directory);
			if (!data->env[i])
				return (perror_return_failure("update env var malloc"));
			return (EXIT_SUCCESS);
		}
		i++;
	}
	ft_printf(CD_ERR_PWD_NOT_FOUND);
	return (EXIT_FAILURE);
}

/**
 * @brief Changes the current working directory. If c->arguments->value is
 * supplied, it will become the new directory. If no parameter is given, the
 * contents of the HOME environment variable will be used.
 * 
 * @param c The current command node
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	builtin_cd(t_commands *c, t_data *data)
{
	char	*new_directory;

	if (c->flags)
		return (ft_printf_exit_code(CD_ERR_FLAGS, EXIT_FAILURE));
	if (c->arguments && c->arguments->next)
		return (ft_printf_exit_code(CD_ERR_EXTRA_ARG, EXIT_FAILURE));
	if (!c->arguments)
	{
		new_directory = get_home_directory(data);
		if (!new_directory)
			return (EXIT_FAILURE);
	}
	else
		new_directory = c->arguments->value;
	// chdir(new_directory);
	if (update_env_variable(new_directory, data))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
