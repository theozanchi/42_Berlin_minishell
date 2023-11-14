/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschott <jschott@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 16:37:00 by jschott           #+#    #+#             */
/*   Updated: 2023/11/13 16:48:52 by jschott          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTER_H
# define EXECUTER_H

# include "../include/Libft/include/libft.h"
# include "../include/Libft/include/get_next_line.h"
# include "../include/Libft/include/ft_printf.h"
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <stdio.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/errno.h>
# include "../pipex.h"

# define BUILTIN_PATH	"./builtin/"

typedef struct s_commands
{
	char				*command;
	char				**arguments;
	char				**flags;
	struct s_commands	*next;
}	t_commands;

typedef struct t_input
{
	char	*path;
}	t_input;

typedef struct t_output
{
	char	*path;
}	t_output;

/* 2_executerr ***************************************************************** */
/*cmd_exectute.c*/
int			cmd_execute(t_commands *cmd, char **env);

/*executer_main.c*/
int			executer(t_output **out_redirect, t_commands *cmds, t_input **in_redirect, char **env);

/*env_extract_path.c*/
char		**env_extract_paths(char **env);
int			cmd_count(t_commands **cmds);

#endif