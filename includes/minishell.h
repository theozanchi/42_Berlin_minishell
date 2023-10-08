/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 12:47:46 by tzanchi           #+#    #+#             */
/*   Updated: 2023/10/08 12:10:12 by tzanchi          ###   ########.fr       */
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
	t_token		*tokens;
}	t_data;

/*_utils_1.c*/
int		perror_return_failure(char *str);
int		ft_printf_exit_code(char *str, int exit_code);
void	ft_tokenlst_addback(t_data *data, t_token *new);

/*exit.c*/
void	free_tokens(t_data *data);
void	free_char_array(char **array);
void	free_memory(t_data *data);
void	exit_minishell(t_data *data, int exit_code);

/*init.c*/
int		init_env(t_data *data, char **env);
int		init_path(t_data *data);
int		init_data(t_data *data, char **env);
void	launch_minishell(t_data *data);

/*lexer.c*/
int		check_arg(char *arg);
t_token	*new_node(char *start_ptr, char *end_ptr);
char	*save_word(t_data *data, char *str);
char	*save_quote(t_data *data, char *str, char quote_symbol);
int		lexer(t_data *data);

/*main.c*/
int		main(int argc, char **argv, char **env);

#endif
