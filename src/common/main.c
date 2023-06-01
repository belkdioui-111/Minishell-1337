/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 00:49:33 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/06/01 12:42:04 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int main(int argc, char const *argv[], char **env)
{
	t_pre_tokens	*head;
	t_user_data		data;
	t_env			*env_head;
	t_command		*head_command;
	char			***all_cmd;
	
	env_head = ft_set_env(env);
	while (1)
	{
		data.user_input = ft_read_input();
		head_command = get_first_command(data.user_input, env_head);
		conver_l_args_to_p(head_command);
		set_path(head_command, env);
		all_cmd = convert_linked_list_to_tr_p(head_command);
		exec(all_cmd, head_command, env);
		// printf_commands(head_command);
		free_commands(&head_command, all_cmd);
		free(data.user_input);
		usleep(50000);print_leaks();
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
