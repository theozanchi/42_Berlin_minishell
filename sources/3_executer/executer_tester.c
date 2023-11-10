/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   executer_tester.c								  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: jschott <jschott@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/11/06 11:00:40 by jschott		   #+#	#+#			 */
/*   Updated: 2023/11/07 14:32:35 by jschott		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "executer.h"

void add_back(t_commands **lst, char *str) {
	t_commands *pos;
	if (!*lst) {
		*lst = (t_commands *)malloc(sizeof(t_commands));
		(*lst)->flags = ft_split(str, ' ');
		(*lst)->command = ft_strdup((*lst)->flags[0]);
		(*lst)->next = NULL;
		return;
	}
	pos = *lst;
	while (pos->next)
		pos = pos->next;
	pos->next = (t_commands *)malloc(sizeof(t_commands));
	pos = pos->next;
	pos->flags = ft_split(str, ' ');
	pos->command = ft_strdup(pos->flags[0]);
	pos->next = NULL;
}

int main(int argc, char **argv, char **env) {
	t_input *in = (t_input *)malloc(sizeof(t_input));
	t_output *out = (t_output *)malloc(sizeof(t_output));
	t_input **inarr = (t_input **)malloc(2 * sizeof(t_output));
	t_output **outarr = (t_output **)malloc(2 * sizeof(t_output));
	t_commands *cmd = NULL; 
	int i = 1;
	in->path = argv[1];
	out->path = argv[argc - 1];
	inarr[0] = in;
	outarr[0] = out;
	inarr[1] = 0;
	outarr[1] = 0;
	while (++i < argc - 1)
		add_back(&cmd, argv[i]);
	// printf("main check cmd:%s", cmd->command);
	executer(outarr, cmd, inarr, env);
	return 1;
}
