/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschott <jschott@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 17:39:15 by tzanchi           #+#    #+#             */
/*   Updated: 2023/12/01 17:32:44 by jschott          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Adds all lines to the the input fd, until delimiter is reached
 * Signals are set to interactive mode
 * 
 * @param write_fd The fd in which to write
 * @param delimiter The delimiter to reach by the here_doc
 */
void	populate_here_doc(int write_fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		signals_interactive();
		line = readline(HERE_DOC_PROMPT);
		signals_non_interactive();
		if (!line)
			break ;
		if (!ft_strcmp(line, delimiter))
		{
			free(line);
			line = NULL;
			break ;
		}
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
	close(write_fd);
}

/**
 * @brief Open an interactive mode for the here_doc, and assign the here_doc as
 * the data->input.fd
 * 
 * @param data The main data structure
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	here_doc(t_data *data)
{
	int	pipe_fds[2];
	int	write_fd;

	if (pipe(pipe_fds) == -1)
		return (perror_return_failure("here_doc pipe"));
	data->input.fd = pipe_fds[0];
	write_fd = pipe_fds[1];
	populate_here_doc(write_fd, data->input.value);
	return (EXIT_SUCCESS);
}
