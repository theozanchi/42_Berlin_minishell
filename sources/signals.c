/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 12:37:12 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/29 16:36:29 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief If SIGINT is received, the readline buffer is cleared and and new
 * prompt is displayed
 * 
 * @param signum The received signal value
 */
void	reset_line(int signum)
{
	(void)signum;
	rl_replace_line("", 0);
	rl_on_new_line();
	write(1, "\n", 1);
	rl_redisplay();
}

/**
 * @brief Displays a new lines and positions the readline offset on it. If
 * SIGQUIT is received, core is dumped
 * 
 * @param signum The received signal value
 */
void	display_new_line(int signum)
{
	if (signum == SIGQUIT)
		ft_printf("Quit (core dumped)");
	write(1, "\n", 1);
	rl_on_new_line();
}

void	ignore_sigint(void)
{
	struct sigaction	sa[2];

	{
		sa[0].sa_handler = SIG_IGN;
		sigemptyset(&sa[0].sa_mask);
		sigaction(SIGINT, &sa[0], NULL);
	}
	{
		sa[1].sa_handler = display_new_line;
		sigemptyset(&sa[1].sa_mask);
		sigaction(SIGQUIT, &sa[1], NULL);
	}
}

/**
 * @brief In interactive mode, SIGQUIT is ignored and the line is reset if
 * SIGINT is received
 * 
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	signals_interactive(void)
{
	struct sigaction	sa[2];

	{
		sa[0].sa_handler = SIG_IGN;
		sigemptyset(&sa[0].sa_mask);
		sa[0].sa_flags = 0;
		if (sigaction(SIGQUIT, &sa[0], NULL))
			return (perror_return_failure("signals interactive sigaction"));
	}
	{
		sa[1].sa_handler = reset_line;
		sigemptyset(&sa[1].sa_mask);
		sa[1].sa_flags = 0;
		if (sigaction(SIGINT, &sa[1], NULL))
			return (perror_return_failure("signals interactive sigaction"));
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Out of the interactive mode, a new line is displayed after each signal
 * and core is dumped if SIGQUIT is received
 * 
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	signals_non_interactive(void)
{
	struct sigaction	sa;

	sa.sa_handler = display_new_line;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa, NULL) || sigaction(SIGINT, &sa, NULL))
		return (perror_return_failure("signals non interactive sigaction"));
	return (EXIT_SUCCESS);
}
