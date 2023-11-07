/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_tester.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschott <jschott@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 11:00:40 by jschott           #+#    #+#             */
/*   Updated: 2023/11/06 13:08:06 by jschott          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"

void	add_back(t_commands *lst, char *str)
{
	t_commands	*pos;

	if (!lst)
	{
		lst = (t_commands *) malloc (sizeof(t_commands));
		lst->command = str;
		lst->flags = str;
		return ;
	}
	pos = lst;
	while (pos->next)
		pos = pos->next;
	pos->next = (t_commands *) malloc (sizeof(t_commands));
	pos = pos->next;
}

int	main (int argc, char **argv, char **env)
{
	t_data		*data = (t_data *) malloc (sizeof(t_commands));
	t_input		*in = (t_input *) malloc (sizeof(argv[1]));
	t_output	*out = (t_output *) malloc (sizeof(argv[argc - 1]));
	t_commands	*cmd;
	int 		i = 1;
	t_commands	*ptr;

	in->path = argv[1];
	out->path = argv[argc - 1];
	ptr = cmd;
	while (++i < argc)
		add_back(ptr, argv[i]);
	executer(&out, cmd, in, env);
	return (1);
}
