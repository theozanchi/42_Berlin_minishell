/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:26:43 by tzanchi           #+#    #+#             */
/*   Updated: 2023/09/12 21:10:04 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	t_data	data;
	int		i;

	(void)argv;
	if (argc != 1)
	{
		perror(ERR_ARG_NR);
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (env[i])
		printf("%s\n", env[i++]);
	ft_memset(&data, 0, sizeof(t_data));
	if (init_data(&data, env) == -1)
		exit_minishell(&data, EXIT_FAILURE);
	else
		launch_minishell(&data);
	return (0);
}
