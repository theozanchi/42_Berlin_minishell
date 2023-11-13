/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschott <jschott@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 12:47:46 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/13 16:09:06 by jschott          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "messages.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/errno.h>
# include <sys/stat.h>
# include <fcntl.h>

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

typedef struct s_list
{
	char			*value;
	struct s_list	*next;
}	t_list;

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
	t_list				*arguments;
	t_list				*flags;
	struct s_commands	*next;
	char				**final;
}	t_commands;

typedef struct s_io
{
	t_type			type;
	char			*value;
	int				fd;
}	t_io;

/* main_data_structure ****************************************************** */
typedef struct s_data
{
	char		**env;
	char		*path;
	char		*argv;
	t_token		*tokens;
	t_commands	*commands;
	t_io		input;
	t_io		output;
}	t_data;

/* 0_utils ****************************************************************** */
/*utils_1.c*/
int			perror_return_failure(char *str);
int			ft_printf_exit_code(char *str, int exit_code);
void		ft_lst_addback(t_list *list, t_list *new);
int			add_new_list_node(t_list **list, char *str);

/* 1_lexer ****************************************************************** */
/*lexer_main.c*/
int			check_arg(char *arg);
int			lexer(t_data *data);

/*lexer_utils.c*/
int			check_end_of_string(char *str);
int			check_double_tokens(char *str);
t_token		*new_token(char *start, char *end, t_type type);
void		ft_tokenlst_addback(t_data *data, t_token *new);

/*save_symbol.c*/
char		*lexer_helper_redirections(char *str, t_token **new);
char		*lexer_helper_dollar_sign(char *str, t_token **new);
char		*save_symbol(t_data *data, char *str);

/*save_word_save_quote.c*/
char		*save_word(t_data *data, char *str);
char		*save_quote(t_data *data, char *str, char quote_symbol);

/* 2_parser ***************************************************************** */
/*concatenate_final_commands.c*/
int			add_command_to_final(size_t *i, t_commands *command_ptr);
int			add_arguments_to_final(size_t *i, t_commands *command_ptr);
int			add_flags_to_final(size_t *i, t_commands *command_ptr);
int			concatenate_successive_commands(t_data *data);

/*parser_main.c*/
int			parser_helper_operands(t_data *data, t_token *token);
int			open_redirection_fd(t_io *redirection, t_token *token, int oflag);
int			parser_helper_redirections(t_data *data, t_token *token);
int			parser(t_data *data);

/*parser_utils.c*/
void		ft_commandlst_addback(t_data *data, t_commands *new);
t_commands	*add_new_command_node(t_data *data);
size_t		get_number_of_command_elements(t_commands *node);

/*populate_node.c*/
int			populate_node_command(t_commands *node, t_token *token);
int			populate_node_flag(t_commands *node, t_token *token);
int			populate_node_argument(t_commands *node, t_token *token);

/* 4_free ***************         ******************************************* */
/*free_1.c*/
void		free_tokens(t_data *data);
void		free_char_array(char **array);
void		free_commands(t_data *data);

/*free_2.c*/
void		free_all_memory(t_data *data);
void		free_memory_between_commands(t_data *data);
void		free_list(t_list *list);
void		free_and_reset_io(t_data *data);

/* ************************************************************************** */

/*init.c*/
int			init_env(t_data *data, char **env);
int			init_path(t_data *data);
void		init_io(t_data *data);
int			init_data(t_data *data, char **env);

/*main.c*/
int			main(int argc, char **argv, char **env);
void		launch_minishell(t_data *data);
void		exit_minishell(t_data *data, int exit_code);

#endif
