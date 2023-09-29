/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 12:47:36 by tzanchi           #+#    #+#             */
/*   Updated: 2023/09/29 11:58:04 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGES_H
# define MESSAGES_H

/*prompts*/
# define ENTRY_PROMPT "\033[1;32mminishell\033[0m> "

/*errors*/
# define ERR_ARG_NR "Minishell does not take any argument\n"
# define ERR_ENV_SETUP "Error occured while initializing environment variable\n"
# define ERR_PATH_SETUP "Error occured while extracting PATH\n"
# define NO_PATH "No path found in env variable\n"
# define ERR_HIST "Failed to add command to history\n"

#endif
