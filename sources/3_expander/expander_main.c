/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 15:54:22 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/13 18:28:22 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	get_variable_expansion_length(char *str)
{
	size_t	length;

	length = -1;
	while (*str && !ft_isspace(*str))
	{
		length++;
		str++;
	}
	return (length);
}

char	*get_exp_var(char *str, size_t var_exp_len, char **env)
{
	while (*env)
	{
		if (!ft_strncmp(str + 1, *env, var_exp_len)
			&& *(*env + var_exp_len) == '=')
			return (*env + var_exp_len + 1);
		env++;
	}
	return ("");
}

char	*concatenate_expanded_string(char **str, size_t *i, t_data *data)
{
	size_t	exp_len;
	char	*s1;
	char	*s2;
	char	*s3;
	char	*expanded_string;

	exp_len = get_variable_expansion_length(&(*str)[*i]);
	s1 = ft_substr(*str, 0, *i);
	s3 = ft_substr(*str, *i + exp_len + 1, ft_strlen(*str) - *i - exp_len);
	if ((*str)[*i + 1] == '?')
		s2 = ft_itoa(data->wstatus);
	else
		s2 = ft_strdup(get_exp_var(&(*str)[*i], exp_len, data->env));
	expanded_string = ft_concat(3, s1, s2, s3);
	*i += ft_strlen(s2);
	free(s1);
	s1 = NULL;
	free(s2);
	s2 = NULL;
	free(s3);
	s3 = NULL;
	return (expanded_string);
}

int	expand_variables(char **str, t_data *data)
{
	size_t	i;
	char	*tmp;

	i = 0;
	while ((*str)[i])
	{
		if ((*str)[i] == '$')
		{
			tmp = concatenate_expanded_string(str, &i, data);
			free(*str);
			*str = tmp;
		}
		else
			i++;
	}
	return (EXIT_SUCCESS);
}

int	expander(t_data *data)
{
	t_commands	*cmd_ptr;
	t_list		*lst_ptr;

	cmd_ptr = data->commands;
	while (cmd_ptr)
	{
		if (cmd_ptr->command_type != SGL_QUOTE)
			expand_variables(&cmd_ptr->command, data);
		lst_ptr = cmd_ptr->arguments;
		while (lst_ptr)
		{
			if (lst_ptr->type != SGL_QUOTE)
				expand_variables(&lst_ptr->value, data);
			lst_ptr = lst_ptr->next;
		}
		lst_ptr = cmd_ptr->flags;
		while (lst_ptr)
		{
			if (lst_ptr->type != SGL_QUOTE)
				expand_variables(&lst_ptr->value, data);
			lst_ptr = lst_ptr->next;
		}
		cmd_ptr = cmd_ptr->next;
	}
	return (EXIT_SUCCESS);
}
