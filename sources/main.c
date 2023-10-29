/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:26:43 by tzanchi           #+#    #+#             */
/*   Updated: 2023/10/29 12:22:58 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Initializes the data needed for the project and launches the program
 * 
 * @param argc Number of arguments, should be 1
 * @param argv Argument variables, just program name
 * @param env environment variables
 * @return EXIT_SUCESS or EXIT_FAILURE
 */
int	main(int argc, char **argv, char **env)
{
	t_data	data;

	(void)argv;
	if (argc != 1)
	{
		errno = EINVAL;
		perror("main");
		exit(EXIT_FAILURE);
	}
	ft_memset(&data, 0, sizeof(t_data));
	if (init_data(&data, env))
		exit_minishell(&data, EXIT_FAILURE);
	else
		launch_minishell(&data);
	return (EXIT_SUCCESS);
}
