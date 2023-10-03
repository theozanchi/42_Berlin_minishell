/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 12:47:46 by tzanchi           #+#    #+#             */
/*   Updated: 2023/10/03 11:46:21 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "messages.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/errno.h>

/*formatting*/
# define GREEN_BOLD "\033[1;32m"

/*data_structure*/
typedef struct s_history
{
	char				*argv;
	struct s_history	*previous;
	struct s_history	*next;
}	t_history;

typedef struct s_token
{
	char			*raw_command;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

typedef struct s_data
{
	char		**env;
	char		*path;
	char		*argv;
	t_history	*history;
	t_token		*tokens;
}	t_data;

/*_utils_1.c*/
int		perror_return_failure(char *str);
void	ft_lstaddback(t_data *data, t_token *new);

/*exit.c*/
void	free_char_array(char **array);
void	free_list(t_data *data);
void	free_memory(t_data *data);
void	exit_minishell(t_data *data, int exit_code);

/*lexer.c*/
int		lexer(t_data *data);

/*init.c*/
int		init_env(t_data *data, char **env);
int		init_data(t_data *data, char **env);
void	launch_minishell(t_data *data);

/*main.c*/
int		main(int argc, char **argv, char **env);

#endif
