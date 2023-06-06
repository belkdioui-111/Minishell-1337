/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 00:49:33 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/06/06 22:29:25 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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

int main(int argc, char const *argv[], char **env)
{
	t_pre_tokens	*head;
	t_user_data		data;
	t_env			*env_head;
	t_command		*head_command;
	char			***all_cmd;
	t_env			*export_head;
	int				is_built;
	
	// signal(SIGINT, sigint_handler);
	// signal(SIGQUIT, sigquit_handler);
	env_head = ft_set_env(env);
	export_head = ft_set_env(env);
		globals.exit_status = 0;
	while (1)
	{
		data.user_input = ft_read_input();
		head_command = get_first_command(data.user_input, env_head);
		if(head_command)
		{
			conver_l_args_to_p(head_command);
			set_path(head_command, env_head);
			all_cmd = convert_linked_list_to_tr_p(head_command);
			is_built = check_if_buil(head_command->cmd, head_command);
			if(head_command && (is_built == 0 || is_built == 11 || is_built == 12 || is_built == 13 || is_built == 14 || is_built == 15 || is_built == 16 || is_built == 17))
				exec(all_cmd, head_command, export_head, env_head);
			else
				exec_built(check_if_buil(head_command->cmd, head_command), head_command, env_head, export_head);
			// printf_commands(head_command);
			free_commands(&head_command);
		}
		free(data.user_input);
		// usleep(50000);print_leaks();
	}
	return 0;
}

// int main(int argc, char const *argv[])
// {
// 	int	fd = open("out.txt", O_CREAT | O_RDONLY | O_WRONLY, 777);
// 	dup2(fd, STDOUT_FILENO);
// 	printf("Hello World 1\n");
// 	close(fd);
// 	return 0;
// }
