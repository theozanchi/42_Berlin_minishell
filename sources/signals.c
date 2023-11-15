/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 12:37:12 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/15 15:21:05 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

int	init_signals(struct sigaction *sa)
{
	{
		sa[0].sa_handler = SIG_IGN;
		sigemptyset(&sa[0].sa_mask);
		sa[0].sa_flags = 0;
		if (sigaction(SIGQUIT, &sa[0], NULL))
			return (EXIT_FAILURE);
	}
	{
		sa[1].sa_handler = signal_handler;
		sigemptyset(&sa[1].sa_mask);
		sa[1].sa_flags = 0;
		if (sigaction(SIGINT, &sa[1], NULL))
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
