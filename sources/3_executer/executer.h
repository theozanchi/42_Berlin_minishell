/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschott <jschott@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 16:37:00 by jschott           #+#    #+#             */
/*   Updated: 2023/11/01 17:51:49 by jschott          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTER_H
# define EXECUTER_H

# include "minishell.h"
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <stdio.h>
# include <sys/wait.h>

# define BUILTIN_PATH	"./builtin/"


t_commands	*cmds_extract_last(t_commands *cmds);
char		**env_extract_paths(char **env);
int			cmd_execute(t_commands *cmd, char **env);


#endif