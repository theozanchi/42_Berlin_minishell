/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:26:43 by tzanchi           #+#    #+#             */
/*   Updated: 2023/10/30 20:35:06 by tzanchi          ###   ########.fr       */
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

/**
 * @brief Input loop, calls lexer, parser and executor then frees memory
 * 
 * @param data Main data structure
 */
void	launch_minishell(t_data *data)
{
	while (1)
	{
		data->argv = readline(ENTRY_PROMPT);
		add_history(data->argv);
		lexer(data);
		parser(data);
		free_memory_between_commands(data);
	}
}

/**
 * @brief Frees and sets all the memory allocated for the programm to NULL and
terminates the process with exit code `exit_code`
 * 
 * @param data Main data structure of type t_data
 * @param exit_code EXIT_SUCCES or EXIT_FAILURE used to exit() the program
 */
void	exit_minishell(t_data *data, int exit_code)
{
	free_all_memory(data);
	rl_clear_history();
	exit(exit_code);
}
