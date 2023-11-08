/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concatenate_final_commands.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzanchi <tzanchi@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 16:31:29 by tzanchi           #+#    #+#             */
/*   Updated: 2023/11/08 18:46:15 by tzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Copies and adds the command in the final command char** array
 * 
 * @param i A pointer to the index where to copy the flags in the char** array
 * @param command_ptr The current command that is being concatenated
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	add_command_to_final(size_t *i, t_commands *command_ptr)
{
	command_ptr->final[*i] = ft_strdup(command_ptr->command);
	if (!command_ptr->final[*i])
		return (perror_return_failure("add_command_to_final malloc"));
	(*i)++;
	return (EXIT_SUCCESS);
}

/**
 * @brief Copies and adds the arguments in the final command char** array
 * 
 * @param i A pointer to the index where to copy the flags in the char** array
 * @param command_ptr The current command that is being concatenated
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	add_arguments_to_final(size_t *i, t_commands *command_ptr)
{
	t_list	*argument;

	argument = command_ptr->arguments;
	while (argument)
	{
		command_ptr->final[*i] = ft_strdup(argument->value);
		if (!command_ptr->final[*i])
			return (perror_return_failure("add_argument_to_final malloc"));
		(*i)++;
		argument = argument->next;
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Copies and adds the flags in the final command char** array
 * 
 * @param i A pointer to the index where to copy the flags in the char** array
 * @param command_ptr The current command that is being concatenated
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	add_flags_to_final(size_t *i, t_commands *command_ptr)
{
	t_list	*flag;

	flag = command_ptr->flags;
	while (flag)
	{
		command_ptr->final[*i] = ft_strdup(flag->value);
		if (!command_ptr->final[*i])
			return (perror_return_failure("add_flag_to_final malloc"));
		(*i)++;
		flag = flag->next;
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Loops through all the arguments to reconcatenate the command in a
 * clean form: command + arguments + flags
 * 
 * @param data The main data structure
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int	concatenate_successive_commands(t_data *data)
{
	t_commands	*command_ptr;
	size_t		number_of_elements;
	size_t		i;

	command_ptr = data->commands;
	while (command_ptr)
	{
		number_of_elements = get_number_of_command_elements(command_ptr);
		command_ptr->final = malloc((number_of_elements + 1) * sizeof(char *));
		if (!command_ptr->final)
			return (perror_return_failure("final command malloc"));
		i = 0;
		if (add_command_to_final(&i, command_ptr)
			|| add_arguments_to_final(&i, command_ptr)
			|| add_flags_to_final(&i, command_ptr))
			return (EXIT_FAILURE);
		command_ptr->final[i] = NULL;
		command_ptr = command_ptr->next;
	}
	return (EXIT_SUCCESS);
}
