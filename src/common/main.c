/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 00:49:33 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/06/21 22:37:20 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_globals	glob;

// void	sigint_handler(int sig_num)
// {
// 	glob.exit_status = 1;
// 	write(1, "\n", 1);
// 	if (glob.in_herdoc == 1)
// 	{
// 		glob.in_herdoc = 3;
// 		close(0);
// 	}
// 	else
// 	{
// 		rl_on_new_line();
// 		rl_replace_line("", 0);
// 		rl_redisplay();
// 	}
// }

int	main(int ac, char *av[], char **env)
{
	t_user_data	data;
	t_env		*env_head;
	t_command	*head_command;
	char		***all_cmd;
	t_env		*export_head;
	int			is_built;

	// signal(SIGINT, sigint_handler);
	// signal(SIGQUIT, SIG_IGN);
	glob.dup = dup(0);
	env_head = ft_set_env(env);
	export_head = ft_set_env(env);
	glob.exit_status = 0;
	glob.in_herdoc = 0;
	glob.env = ft_set_env(env);
	glob.export = ft_set_env(env);
	while (1)
	{
		dup2(glob.dup, 0);
		data.user_input = ft_read_input();
		
		head_command = get_first_command(data.user_input, env_head);
		if (head_command)
		{
			conver_l_args_to_p(head_command);
			all_cmd = convert_linked_list_to_tr_p(head_command);
			exec(all_cmd, head_command);
			// printf_commands(head_command);
			// if (head_command->path)
			
			free_all_cmd(all_cmd);
			free_commands(&head_command);
		}
		free(data.user_input);
		// print_leaks();
	}
	return (0);
}
