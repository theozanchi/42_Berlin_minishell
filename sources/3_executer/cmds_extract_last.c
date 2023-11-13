/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds_extract_last.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschott <jschott@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 16:34:51 by jschott           #+#    #+#             */
/*   Updated: 2023/11/13 16:50:59 by jschott          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "executer.h"
#include "minishell.h"

t_commands	*cmds_extract_last(t_commands *cmds)
{
	t_commands	*ptr;
	t_commands	*last;

	if (!cmds)
		return (0);
	if (!cmds->next)
		return (cmds);
	ptr = cmds;
	while (ptr->next->next)
		ptr = ptr->next;
	last = ptr->next;
	ptr->next = 0;
	return (last);
}
