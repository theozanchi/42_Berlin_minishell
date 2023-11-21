/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 12:57:20 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/21 13:01:21 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPORT_H
# define EXPORT_H

# include "minishell.h"

# define VALID_CHARACTERS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\
							0123456789_"
# define EXPORT_ERR_FLAGS "minishell: export: no options supported\n"
# define EXPORT_INV_IDEN "minishell: export: '%s': not a valid identifier\n"

char	*get_str_from_env(char *str, t_data *data);
char	*extract_value(char *str);
char	*extract_identifier(char *str);
char	*get_and_check_identifier(char *str);
void	overwrite_env_variable(char *id, char *value, t_data *data);
int		add_variable_to_env(char *id, char *value, t_data *data);

#endif
