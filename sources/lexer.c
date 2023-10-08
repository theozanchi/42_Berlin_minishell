/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 11:49:34 by tzanchi           #+#    #+#             */
/*   Updated: 2023/10/08 12:16:55 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*Loops through the argument string to assess its validity. If the string is
invalid, prints an error message and returns EXIT_FAILURE. The following checks
are performed:
• the string does not begin by a pipe '|'
• all quoted arguments (with ' or ") are properly closed*/
int	check_arg(char *arg)
{
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

/*Allocates memory for a new t_token node, then allocates enough memory to store
the raw command contained between the pointers start_ptr and end_ptr and copies
the raw command into new_raw_command. If a malloc() fails, NULL is returned*/
t_token	*new_node(char *start_ptr, char *end_ptr)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
	{
		perror("new_node()");
		return (NULL);
	}
	new->raw_command = malloc((end_ptr - start_ptr + 1) * sizeof(char));
	if (!new->raw_command)
	{
		perror("raw_command in new_node()");
		return (NULL);
	}
	ft_strlcpy(new->raw_command, start_ptr, end_ptr - start_ptr + 1);
	new->next = NULL;
	return (new);
}

/*Creates a new node to save the first word contained in the string 'str'. If
new_node() fails, NULL is returned. The function returns a ptr to the end of the
word (either the first space after it or the NULL terminator)*/
char	*save_word(t_data *data, char *str)
{
	char	*end_ptr;
	t_token	*new;

	end_ptr = str;
	while (!ft_isspace(*end_ptr) && *end_ptr)
		end_ptr++;
	new = new_node(str, end_ptr);
	if (!new)
		return (NULL);
	ft_tokenlst_addback(data, new);
	return (end_ptr);
}

/*Creates a new node to save the quoted string contained in the string 'str',
quote_symbol being the symbol used (' or "). If new_node() fails, NULL is
returned. The function returns a ptr to the end of the quoted string (either the
first space after it or the NULL terminator)*/
char	*save_quote(t_data *data, char *str, char quote_symbol)
{
	char	*end_ptr;
	t_token	*new;

	end_ptr = str;
	while (*end_ptr != quote_symbol)
		end_ptr++;
	new = new_node(str, end_ptr);
	if (!new)
		return (NULL);
	ft_tokenlst_addback(data, new);
	return (end_ptr + 1);
}

/*Loops through the string data->argv to check if it is properly formatted. If
so it stores in a new t_token lists all the tokens by properly splitting words
and quoted arguments. Returns EXIT_FAILURE if the argument is invalid.*/
int	lexer(t_data *data)
{
	char	*ptr;

	ptr = data->argv;
	if (check_arg(data->argv))
		return (EXIT_FAILURE);
	while (*ptr)
	{
		if (ft_isspace(*ptr))
			ptr++;
		else if (*ptr == '\'' || *ptr == '\"')
			ptr = save_quote(data, ptr + 1, *ptr);
		else
			ptr = save_word(data, ptr);
	}
	return (EXIT_SUCCESS);
}
