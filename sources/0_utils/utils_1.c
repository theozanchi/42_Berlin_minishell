/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 08:59:15 by tzanchi           #+#    #+#             */
/*   Updated: 2023/10/30 20:08:18 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Calls the function perror() with argument 'str'
 * 
 * @param str Error message to print
 * @return EXIT_FAILURE
 */
int	perror_return_failure(char *str)
{
	perror(str);
	return (EXIT_FAILURE);
}

/**
 * @brief Calls the function ft_printf() with argument 'str' and returns
'exit_code'
 * 
 * @param str Message to print
 * @param exit_code EXIT_SUCCESS or EXIT_FAILURE
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	ft_printf_exit_code(char *str, int exit_code)
{
	ft_printf(str);
	return (exit_code);
}
