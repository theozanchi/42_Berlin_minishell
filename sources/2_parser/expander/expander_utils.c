/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 19:05:56 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/13 19:47:00 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Get the length of the variable expansion tag (i.e. what follows the $
 * sign: the length of $TEST is 4)
 * 
 * @param str Pointer to the dollar sign, marking the beginning of the value to
 * expand
 * @return The length of the variable expansion tag
 */
ssize_t	get_variable_expansion_length(char *str)
{
	ssize_t	length;

	if (*(str + 1) == '?')
		return (1);
	length = -1;
	while (*str && !ft_isspace(*str))
	{
		length++;
		str++;
	}
	return (length);
}

/**
 * @brief Loops through the current env array to locate the variable to expand
 * 
 * @param str Pointer to the dollar sign marking the beginning of the variable
 * @param var_exp_len The length of the word to expand
 * @param env The current environment
 * @return A pointer to the expanded variable, or an empty string if no
 * corresponding variable is found
 */
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

/**
 * @brief Expands the first variable of the string str, located at index i, then
 * updates i to indicate the end of the expanded value
 * 
 * @param str Pointer to the beginning of the string
 * @param i Index of the first dollar to expand in the string str
 * @param data The main data structure to access the env variable
 * @return Pointer to the beginning of the new string, where the variable has
 * been expanded or NULL in case of failure
 */
char	*concatenate_expanded_string(char **str, size_t *i, t_data *data)
{
	ssize_t	exp_len;
	char	*s1;
	char	*s2;
	char	*s3;
	char	*expanded_string;

	exp_len = get_variable_expansion_length(&(*str)[*i]);
	if (!exp_len)
		return (NULL);
	s1 = ft_substr(*str, 0, *i);
	s3 = ft_substr(*str, *i + exp_len + 1, ft_strlen(*str) - *i - exp_len);
	if ((*str)[*i + 1] == '?')
		s2 = ft_itoa(data->wstatus);
	else
		s2 = ft_strdup(get_exp_var(&(*str)[*i], exp_len, data->env));
	expanded_string = ft_concat(3, s1, s2, s3);
	if (!expanded_string)
		return (NULL);
	*i += ft_strlen(s2);
	free_utils_strings(&s1, &s2, &s3);
	return (expanded_string);
}

/**
 * @brief Free the utils strings used to expand the variables
 * 
 * @param s1 First half of the string
 * @param s2 Expanded variable string
 * @param s3 Second half of the string
 */
void	free_utils_strings(char **s1, char **s2, char **s3)
{
	if (*s1)
	{
		free(*s1);
		*s1 = NULL;
	}
	if (*s2)
	{
		free(*s2);
		*s2 = NULL;
	}
	if (*s3)
	{
		free(*s3);
		*s3 = NULL;
	}
}
