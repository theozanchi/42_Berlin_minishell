/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 11:49:34 by tzanchi           #+#    #+#             */
/*   Updated: 2023/10/03 11:43:09 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_node(char *arg)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
	{
		perror("Token malloc");
		return (NULL);
	}
	new->raw_command = ft_strdup(arg);
	new->next = NULL;
}

ssize_t	get_arg_length(char **split)
{
	ssize_t	length;

	length = 0;
	while (*split)
	{
		length += ft_strlen(*split);
		split++;
		if (*split && (**split == '\'' || **split == '\"'))
			break ;
		else if (*split)
			length++;
		else
		{
			errno = EINVAL;
			return (-1);
		}
	}
	return (length);
}

int	save_quoted_arg(t_data *data, char ***split)
{
	ssize_t	length;

	length = get_arg_length(*split);
	if (length == -1)
		return (perror_return_failure())
	return (EXIT_SUCCESS);
}

int	lexer(t_data *data)
{
	char	*split;
	char	*ptr;
	t_token	*new;

	split = ft_split(data->argv, " ");
	if (!split)
		return (perror_return_failure("lexer ft_split"));
	ptr = split;
	data->tokens = malloc(sizeof(t_token));
	if (!data->tokens)
		return (perror_return_failure("data->tokens malloc"));
	data->tokens = NULL;
	while (ptr)
	{
		if (*ptr == '\'' || *ptr == '\"')
		{
			if (save_quoted_arg(data, &++ptr))
				return (EXIT_FAILURE);
		}
		else
		{
			new = new_node(*ptr++);
			if (!new)
				return (EXIT_FAILURE);
			ft_lstaddback(data, new);
		}
	}
	free(split);
}
