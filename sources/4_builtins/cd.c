/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 18:17:46 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/21 22:12:40 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define CD_ERR_HOME_NOT_FOUND "minishell: cd: HOME directory not found in env\n"
#define CD_ERR_OLDPWD_NOT_FOUND "minishell: cd: OLDPWD directory not found in \
									env\n"
#define CD_ERR_PWD_NOT_FOUND "minishell: cd: PWD directory not found in env\n"
#define CD_ERR_FLAGS "minishell: cd: no options supported\n"
#define CD_ERR_EXTRA_ARG "minishell: cd: too many arguments\n"

/**
 * @brief Checks that no flag and max one argument are passed to the builtin
 * 
 * @param c The current command node
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int	check_arg(t_commands *c)
{
	if (c->flags)
		return (ft_printf_exit_code(CD_ERR_FLAGS, EXIT_FAILURE));
	if (c->arguments && c->arguments->next)
		return (ft_printf_exit_code(CD_ERR_EXTRA_ARG, EXIT_FAILURE));
	return (EXIT_SUCCESS);
}

static char	*get_new_path(t_commands *c, t_data *data)
{
	char	*path;

	if (!c->arguments)
	{
		path = ft_getenv("HOME=", data);
		if (!path)
		{
			ft_printf(CD_ERR_HOME_NOT_FOUND);
			return (path);
		}
		path += 5;
	}
	else if (!ft_strcmp(c->arguments->value, "-"))
	{
		path = ft_getenv("OLDPWD=", data);
		if (!path)
		{
			ft_printf(CD_ERR_OLDPWD_NOT_FOUND);
			return (path);
		}
		path += 7;
	}
	else
		path = c->arguments->value;
	return (path);
}

static int	update_env(char *new_path, char *old_path, t_data *data)
{
	if (ft_getenv("PWD=", data))
	{
		if (overwrite_env_variable("PWD=", new_path, data))
			return (EXIT_FAILURE);
	}
	if (ft_getenv("OLDPWD=", data))
	{
		if (overwrite_env_variable("OLDPWD=", old_path, data))
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
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
	char	old_pwd[PATH_MAX];
	char	*path;
	char	new_pwd[PATH_MAX];

	if (check_arg(c))
		return (EXIT_FAILURE);
	if (!getcwd(old_pwd, sizeof(old_pwd)))
		return (perror_return_failure("getcwd() error"));
	path = get_new_path(c, data);
	if (!path)
		return (EXIT_FAILURE);
	if (chdir(path))
		return (perror_return_failure("chdir() error"));
	if (!getcwd(new_pwd, sizeof(new_pwd)))
		return (perror_return_failure("getcwd() error"));
	if (update_env(new_pwd, old_pwd, data))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
