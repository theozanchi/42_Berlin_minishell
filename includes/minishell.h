/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 12:47:46 by tzanchi           #+#    #+#             */
/*   Updated: 2023/09/11 16:25:07 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "messages.h"

/*formatting*/
# define GREEN_BOLD "\033[1;32m"

/*data_structure*/
typedef struct s_data
{
	char	**env;
	char	*argv;
}	t_data;

/*exit.c*/
void	free_memory(t_data *data);

/*main.c*/
int		main(int argc, char **argv, char **env);

#endif
