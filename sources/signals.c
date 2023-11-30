/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 12:37:12 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/30 16:56:12 by tzanchi          ###   ########.fr       */
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
	write(1, "\n", STDERR_FILENO);
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
	write(1, "\n", STDERR_FILENO);
	rl_on_new_line();
}

/**
 * @brief In interactive mode, SIGQUIT is ignored and the line is reset if
 * SIGINT is received
 * 
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
void	signals_interactive(void)
{
	signal(SIGINT, reset_line);
	signal(SIGQUIT, SIG_IGN);
}

/**
 * @brief Out of the interactive mode, a new line is displayed after each signal
 * and core is dumped if SIGQUIT is received
 * 
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
void	signals_non_interactive(void)
{
	signal(SIGINT, display_new_line);
	signal(SIGQUIT, display_new_line);
}
