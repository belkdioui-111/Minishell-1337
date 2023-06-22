/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_first_command_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 19:21:59 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/06/22 16:57:01 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	free_linked(t_pre_tokens **head)
{
	t_pre_tokens	*node;
	t_pre_tokens	*next;

	node = *head;
	while (node)
	{
		next = node->next;
		free(node->content);
		free(node);
		node = next;
	}
}

void	free_commands(t_command **head)
{
	t_command	*command;
	t_command	*command_next;

	command = *head;
	while (command)
	{
		command_next = command->next;
		free(command->path);
		free_double(command->db_args);
		free_linked(&(command->args));
		free_linked(&(command->input_files));
		free_linked(&(command->output_files));
		free_linked(&(command->append_files));
		free_linked(&(command->herdoc_files));
		if (command->here_doc_data)
			close(command->pipe_hd);
		if (command->here_doc_data)
			free(command->here_doc_data);
		if (command->cmd)
			free(command->cmd);
		if (command->cmd)
			command->cmd = NULL;
		free(command);
		command = command_next;
	}
}
