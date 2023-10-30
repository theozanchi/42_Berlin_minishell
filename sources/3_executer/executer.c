/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschott <jschott@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 16:22:45 by jschott           #+#    #+#             */
/*   Updated: 2023/10/30 16:45:52 by jschott          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executer.h"

int	executer(t_output *output, t_commands *cmds, t_input *input)
{
	int	fd_out;

	while (output[i + 1])
	{
		fd_out = open(output[i], O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (!output[++i])
			break ;
		close(fd_out);
    }
	close(fd_out);
}
