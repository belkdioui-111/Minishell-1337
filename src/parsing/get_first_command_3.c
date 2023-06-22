/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_first_command_3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabrahm <ylabrahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 19:24:21 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/06/21 19:24:36 by ylabrahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	print_leaks(void)
{
	printf("\n\n");
	printf("-----------[Leaks]-----------\n");
	system("leaks minishell | grep -A20 'leaks Report Version: 4.0'");
}

void	printf_linked(t_pre_tokens *head)
{
	t_pre_tokens	*node;
	int				i;

	i = 0;
	node = head;
	while (node)
	{
		printf("[%s]", node->content);
		node = node->next;
	}
	printf("\n");
}

void	printf_commands(t_command *head)
{
	t_command	*temp_comm;

	temp_comm = head;
	printf("--------------------------\n");
	while (temp_comm)
	{
		printf("command : [%s]\n", temp_comm->cmd);
		printf("Args :");
		printf_linked(temp_comm->args);
		printf("Out-Files : ");
		printf_linked(temp_comm->output_files);
		printf("In-Files : ");
		printf_linked(temp_comm->input_files);
		printf("Append-Files : ");
		printf_linked(temp_comm->append_files);
		printf("Herdoc-Files : ");
		printf_linked(temp_comm->herdoc_files);
		printf("--------------------------\n");
		printf("in_error : %d\n", temp_comm->in_error);
		printf("--------------------------\n");
		temp_comm = temp_comm->next;
	}
}

void	printf_env(t_env *head)
{
	t_env	*node;

	node = head;
	while (node)
	{
		printf("%s=%s\n", node->index, node->value);
		node = node->next;
	}
}
