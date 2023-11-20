/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 11:49:34 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/20 19:39:29 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks that the user input is not just spaces
 * 
 * @param arg The user input
 * @return 0 or 1
 */
static int	is_just_spaces(char *arg)
{
	while (*arg)
	{
		if (!ft_isspace(*arg++))
			return (0);
	}
	return (1);
}

/**
 * @brief Loops through the argument string to assess its validity (not starting
 with pipe, all quotes are closed)
 * 
 * @param arg User input
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	check_user_arg(char *arg)
{
	if (!arg)
		return (EXIT_FAILURE);
	if (is_just_spaces(arg))
		return (EXIT_FAILURE);
	while (ft_isspace(*arg))
		arg++;
	if (*arg == '|')
		return (ft_printf_exit_code(UNEXPECTED_PIPE, EXIT_FAILURE));
	while (*arg)
	{
		if (*arg == '\'' || *arg == '\"')
		{
			arg = ft_strchr(arg + 1, *arg);
			if (arg == NULL)
				return (ft_printf_exit_code(UNCLOSED_QUOTE, EXIT_FAILURE));
			else
				arg++;
		}
		else
			arg++;
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Loops through the string data->argv to check if it is properly
formatted. If so it stores in a new t_token lists all the tokens by properly
splitting words and quoted arguments. 
 * 
 * @param data Main data structure of type t_data
 * @return EXIT_SUCESS or EXIT_FAILURE
 */
int	lexer(t_data *data)
{
	char	*ptr;

	ptr = data->argv;
	if (check_user_arg(data->argv))
		return (EXIT_FAILURE);
	while (*ptr)
	{
		if (ft_isspace(*ptr))
			ptr++;
		else if (ft_strchr(SUPPORTED_SYMBOLS, *ptr))
			ptr = save_symbol(data, ptr);
		else if (*ptr == '\'' || *ptr == '\"')
			ptr = save_quote(data, ptr + 1, *ptr);
		else
			ptr = save_word(data, ptr);
		if (ptr == NULL)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
