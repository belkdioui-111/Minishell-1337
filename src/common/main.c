/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 00:49:33 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/06/22 14:35:27 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_globals	g_glob;

// void	sigint_handler(int sig_num)
// {
// 	g_glob.exit_status = 1;
// 	write(1, "\n", 1);
// 	if (g_glob.in_herdoc == 1)
// 	{
// 		g_glob.in_herdoc = 3;
// 		close(0);
// 	}
// 	else
// 	{
// 		rl_on_new_line();
// 		rl_replace_line("", 0);
// 		rl_redisplay();
// 	}
// }

void	if_cmd_true(t_command	*head_command)
{
	char		***all_cmd;

	if (head_command)
	{
		conver_l_args_to_p(head_command);
		all_cmd = convert_linked_list_to_tr_p(head_command);
		exec(all_cmd, head_command);
		free_all_cmd(all_cmd);
		free_commands(&head_command);
	}
}

int	main(int ac, char *av[], char **env)
{
	t_user_data	data;
	t_env		*env_head;
	t_command	*head_command;
	int			is_built;

	// signal(SIGINT, sigint_handler);
	// signal(SIGQUIT, SIG_IGN);
	g_glob.dup = dup(0);
	env_head = ft_set_env(env);
	g_glob.exit_status = 0;
	g_glob.in_herdoc = 0;
	g_glob.env = ft_set_env(env);
	g_glob.export = ft_set_env(env);
	while (1)
	{
		dup2(g_glob.dup, 0);
		data.user_input = ft_read_input();
		head_command = get_first_command(data.user_input, env_head);
		if_cmd_true(head_command);
		free(data.user_input);
		// print_leaks();
	}
	return (0);
}
