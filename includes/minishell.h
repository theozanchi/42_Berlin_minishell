/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 12:47:46 by tzanchi           #+#    #+#             */
/*   Updated: 2023/12/01 18:02:17 by tzanchi          ###   ########.fr       */
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
# include <sys/wait.h>
# include <fcntl.h>
# include <signal.h>
# include <unistd.h>
# include <limits.h>

/*formatting*/
# define GREEN_BOLD "\033[1;32m"
# define RED_BOLD "\033[1;31m"

/*program parameters*/
# define SUPPORTED_SYMBOLS "<|>"
# define FDX_NONE -1
# define FDX_OR 0
# define FDX_OW 1
# define FDX_RW 2

/*data_structure*/
typedef enum e_type
{
	OPERAND,
	PIPE,
	INPUT,
	OUTPUT,
	APPEND,
	HERE_DOC,
	STDIN,
	STDOUT,
}	t_type;

typedef enum e_quote
{
	NONE,
	SGL,
	DBL
}	t_quote;

typedef struct s_list
{
	char			*value;
	t_quote			quote;
	struct s_list	*next;
}	t_list;

/* lexer ******************************************************************** */
typedef struct s_token
{
	t_type			type;
	t_quote			quote;
	char			*value;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

/* parser ******************************************************************* */
typedef struct s_commands
{
	char				*command;
	t_quote				cmd_quote;
	t_list				*arguments;
	t_list				*flags;
	struct s_commands	*next;
	char				**final;
}	t_commands;

typedef struct s_io
{
	t_type			type;
	t_quote			quote;
	char			*value;
	int				fd;
}	t_io;

/* main_data_structure ****************************************************** */
typedef struct s_data
{
	char		**env;
	char		*path;
	int			wstatus;
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
int			add_new_list_node(t_list **list, t_token *token);

/* 1_lexer ****************************************************************** */
/*lexer_main.c*/
int			check_user_arg(char *arg);
int			lexer(t_data *data);

/*lexer_utils.c*/
int			check_end_of_string(char *str);
int			check_double_tokens(char *str);
t_token		*new_token(char *start, char *end, t_type type, t_quote quote);
void		ft_tokenlst_addback(t_data *data, t_token *new);

/*save_symbol.c*/
char		*lexer_helper_redirections(char *str, t_token **new);
char		*save_symbol(t_data *data, char *str);

/*save_word_save_quote.c*/
char		*save_word(t_data *data, char *str);
char		*save_quote(t_data *data, char *str, char quote_symbol);

/* 2_parser ***************************************************************** */
/* expander */
/*expander_main.c*/
int			expand_string(char **str, t_data *data);
int			expand_command(t_commands *node, t_data *data);
int			expand_list_of_str(t_list *list, t_data *data);
int			expander(t_data *data);

/*expander_utils.c*/
ssize_t		get_variable_expansion_length(char *str);
char		*get_exp_var(char *str, size_t var_exp_len, char **env);
char		*concatenate_expanded_string(char **str, size_t *i, t_data *data);
void		free_utils_strings(char **s1, char **s2, char **s3);

/* parser */
/*concatenate_final_commands.c*/
int			add_command_to_final(size_t *i, t_commands *command_ptr);
int			add_arguments_to_final(size_t *i, t_commands *command_ptr);
int			add_flags_to_final(size_t *i, t_commands *command_ptr);
int			concatenate_successive_commands(t_data *data);

/*parser_helper_export_builtin.c*/
t_list		*get_current_argument(t_commands *node);
char		*get_equal_sign_pos(t_commands *node);
int			helper_quote_var(t_commands *node, t_token *token, \
							char quote_sign, int *add_new_node);
int			helper_export_builtin(t_commands *node, t_token *token);

/*parser_main.c*/
int			parser_helper_operands(t_data *data, t_token *token, \
									int *create_new_node);
int			open_redirection_fd(t_data *data, t_io *redir, t_token *token, \
								int oflag);
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

/*here_doc*/
void		populate_here_doc(int write_fd, char *delimiter);
int			here_doc(t_data *data);

/* 3_executer *************************************************************** */
/*executer_main.c*/
int			executer(t_data *data);
int			cmd_count(t_commands *cmds);

/*pipes_creator.c*/
int			*build_pipes(int fd_out, int fd_in, int cmds_num);

/*pipeline_executer.c*/
int			execute_pipeline(int *fd_pipes, pid_t *pid, t_data *data);
int			close_fd(int *fd);

/*fd_manager.c*/
int			close_fd(int *fd);
int			close_unused_fd(int *fd_pipes, int pos, int keep, int len);

/*command_executer.c*/
int			command_executer(t_commands *cmd, t_data *data);
char		*search_cmd_path(t_commands *cmd, char **env);
char		**env_extract_paths(char **env);

/* 4_builtins *************************************************************** */
/*builtins_utils.c*/
int			cmd_is_a_builtin(t_commands *node);
int			launch_builtin(t_commands *c, t_data *data);
char		*ft_getenv(char *str, t_data *data);
int			overwrite_env_variable(char *id, char *value, t_data *data);
int			add_variable_to_env(char *id, char *value, t_data *data);

/*builtins*/
int			builtin_cd(t_commands *c, t_data *data);
int			builtin_echo(t_commands *c);
int			builtin_env(t_commands *c, t_data *data);
int			builtin_exit(t_commands *c, t_data *data);
int			builtin_export(t_commands *c, t_data *data);
int			builtin_pwd(t_commands *c, t_data *data);
int			builtin_unset(t_commands *c, t_data *data);

/* 5_free ******************************************************************* */
/*free_1.c*/
void		free_tokens(t_data *data);
void		free_char_array(char **array);
void		free_commands(t_data *data);

/*free_2.c*/
void		free_all_memory(t_data *data);
void		free_memory_between_commands(t_data *data);
void		free_list(t_list *list);
void		free_and_reset_io(t_data *data);
int			reverse_free_char_array(char **array, size_t i, int exit_code);

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

/*signals.c*/
void		reset_line(int signum);
void		display_new_line(int signum);
void		signals_interactive(void);
void		signals_non_interactive(void);

#endif
