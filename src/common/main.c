/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 00:49:33 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/06/20 19:05:52 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_globals	glob;
// void	sigint_handler(int sig_num)
// {
// 	write(2, "\n", 1);
// 	rl_clear_history();
// 	rl_replace_line("", 0);
// 	rl_on_new_line();
// 	rl_redisplay();
// 	signal(SIGINT, sigint_handler);
// }

// void	sigquit_handler(int sig_num)
// {
// 	signal(SIGQUIT, sigquit_handler);
// }

int	main(int argc, char const *argv[], char **env)
{
	t_user_data	data;
	t_env		*env_head;
	t_command	*head_command;
	char		***all_cmd;
	t_env		*export_head;

	// signal(SIGINT, sigint_handler);
	// signal(SIGQUIT, sigquit_handler);
	glob.env = ft_set_env(env);
	glob.export = ft_set_env(env);
	glob.exit_status = 0;
	while (1)
	{
		data.user_input = ft_read_input();
		head_command = get_first_command(data.user_input, glob.env);
		if (head_command)
		{
			conver_l_args_to_p(head_command);
			all_cmd = convert_linked_list_to_tr_p(head_command);
			exec(all_cmd, head_command);
			// printf_commands(head_command);
			free_commands(&head_command);
		}
		free(data.user_input);
		// usleep(50000);print_leaks();
	}
	return (0);
}
