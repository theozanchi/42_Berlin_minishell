/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 12:47:46 by tzanchi           #+#    #+#             */
/*   Updated: 2023/10/30 21:03:24 by tzanchi          ###   ########.fr       */
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
# define SUPPORTED_SYMBOLS "<|>$"

/*data_structure*/
typedef enum e_type
{
	OPERAND,
	SGL_QUOTE,
	DBL_QUOTE,
	PIPE,
	INPUT,
	OUTPUT,
	APPEND,
	HERE_DOC,
	STDIN,
	STDOUT,
	EXIT_CODE,
	ENV_VARIABLE,
}	t_type;

/* lexer ******************************************************************** */
typedef struct s_token
{
	t_type			type;
	char			*value;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

/* parser ******************************************************************* */
typedef struct s_commands
{
	char				*command;
	char				*argument;
	char				**flags;
	struct s_commands	*next;
}	t_commands;

typedef struct s_input
{
	t_type			type;
	char			*path;
	struct s_input	*next;
}	t_input;

typedef struct s_output
{
	t_type			type;
	char			*path;
	struct s_output	*next;
}	t_output;

/* main_data_structure ****************************************************** */
typedef struct s_data
{
	char			**env;
	char			*path;
	char			*argv;
	t_token			*tokens;
	t_commands		*commands;
	t_input			*input;
	t_output		*output;
}	t_data;

/* 0_utils ****************************************************************** */
/*utils_1.c*/
int		perror_return_failure(char *str);
int		ft_printf_exit_code(char *str, int exit_code);

/* 1_lexer ****************************************************************** */
/*lexer_main.c*/
int		check_arg(char *arg);
int		lexer(t_data *data);

/*lexer_utils.c*/
int		check_end_of_string(char *str);
int		check_double_tokens(char *str);
t_token	*new_token(char *start, char *end, t_type type);
void	ft_tokenlst_addback(t_data *data, t_token *new);

/*save_symbol.c*/
char	*lexer_helper_redirections(char *str, t_token **new);
char	*lexer_helper_dollar_sign(char *str, t_token **new);
char	*save_symbol(t_data *data, char *str);

/*save_word_save_quote.c*/
char	*save_word(t_data *data, char *str);
char	*save_quote(t_data *data, char *str, char quote_symbol);

/* 2_parser ***************************************************************** */
/*parser_main.c*/
int		parser_helper_operands(t_data *data, t_token *token);
int		parser_helper_redirections(t_data *data, t_token *token);
int		input_output_lists_init(t_data *data);
int		parser(t_data *data);

/*parser_utils.c*/
void	ft_inputlst_addback(t_data *data, t_input *new);
int		add_new_input_node(t_data *data, t_token *token);
void	ft_outputlst_addback(t_data *data, t_output *new);
int		add_new_output_node(t_data *data, t_token *token);

/* 4_free ***************         ******************************************* */
/*free_1.c*/
void	free_tokens(t_data *data);
void	free_char_array(char **array);
void	free_input(t_data *data);
void	free_output(t_data *data);
void	free_commands(t_data *data);

/*free_2.c*/
void	free_all_memory(t_data *data);
void	free_memory_between_commands(t_data *data);

/* ************************************************************************** */

/*init.c*/
int		init_env(t_data *data, char **env);
int		init_path(t_data *data);
int		init_data(t_data *data, char **env);

/*main.c*/
int		main(int argc, char **argv, char **env);
void	launch_minishell(t_data *data);
void	exit_minishell(t_data *data, int exit_code);

#endif
