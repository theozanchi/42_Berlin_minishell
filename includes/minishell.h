/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 12:47:46 by tzanchi           #+#    #+#             */
/*   Updated: 2023/10/17 12:53:59 by tzanchi          ###   ########.fr       */
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

/*program parameters*/
# define SUPP_SYMBOLS "<|>$"

/*data_structure*/
typedef enum e_token_type
{
	ARG,
	PIPE,
	INPUT,
	OUTPUT,
	APPEND,
	HERE_DOC,
	EXIT_STATUS,
	ENV_VAR,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
	BUILTIN_ECHO,
	BUILTIN_CD,
	BUILTIN_PWD,
	BUILTIN_EXPORT,
	BUILTIN_UNSET,
	BUILTIN_ENV,
	BUILTIN_EXIT,
	CTRL_C,
	CTRL_D,
	CTRL_BACKSLASH
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
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

/* 0_utils ****************************************************************** */
/*utils_1.c*/
int		perror_return_failure(char *str);
int		ft_printf_exit_code(char *str, int exit_code);
void	ft_tokenlst_addback(t_data *data, t_token *new);

/* 1_lexer ****************************************************************** */
/*lexer_main.c*/
int		check_arg(char *arg);
t_token	*new_node(char *start, char *end, t_token_type type);
char	*save_word(t_data *data, char *str);
char	*save_quote(t_data *data, char *str, char quote_symbol);
int		lexer(t_data *data);
/*save_symbol.c*/
int		check_double_tokens(char *str);
char	*helper_redirections(char *str, t_token **new);
char	*helper_dollar_sign(char *str, t_token **new);
char	*save_symbol(t_data *data, char *str);

/* 2_parser ***************************************************************** */

/* ************************************************************************** */

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

/*main.c*/
int		main(int argc, char **argv, char **env);

/*parser.c*/

#endif
