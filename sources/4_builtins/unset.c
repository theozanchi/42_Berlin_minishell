/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 13:11:23 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/16 13:14:29 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define UNSET_ERR_FLAGS "minishell: unset: no options supported\n"

int	builtin_unset(t_commands *c, t_data *data)
{
	if (c->flags)
		return (ft_printf_exit_code(UNSET_ERR_FLAGS, EXIT_SUCCESS))
}
