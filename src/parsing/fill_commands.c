/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabrahm <ylabrahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 16:13:40 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/06/15 16:24:46 by ylabrahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_command	*get_last_command(t_command **input)
{
	t_command	*last_command;

	last_command = *input;
	while (last_command->next)
		last_command = last_command->next;
	return (last_command);
}

void	set_null_memeber(t_command **command, int state)
{
	t_command	*new_command;

	if (state)
	{
		new_command = *command;
		new_command->args->prev = NULL;
	}
	new_command->next = NULL;
	new_command->cmd = NULL;
	new_command->output_files = NULL;
	new_command->input_files = NULL;
	new_command->append_files = NULL;
	new_command->herdoc_files = NULL;
	new_command->in_type = 0;
	new_command->out_type = 0;
	new_command->here_doc_data = NULL;
	new_command->has_error = 0;
	new_command->in_error = 0;
}

t_pre_tokens	*add_to_command(t_pre_tokens *node, t_command **f_command)
{
	t_command		*new_command;
	t_command		*last_command;
	t_pre_tokens	*temp_node;
	t_pre_tokens	*ret_node;
	t_pre_tokens	*temp_free;

	new_command = malloc(sizeof(t_command));
	if (!new_command)
		return 0;
	temp_node = node;
	new_command->args = node;
	set_null_memeber(&new_command, 1);
	while ((temp_node->next) && (temp_node->next->type != TYPE_RED_PIP))
		temp_node = temp_node->next;
	ret_node = temp_node->next;
	if (temp_node)
		temp_node->next = 0;
	if (!(*f_command))
		*f_command = new_command;
	else
	{
		last_command = get_last_command(f_command);
		last_command->next = new_command;
	}
	return (ret_node);
}

t_command	*ft_fill_commands(t_pre_tokens **head)
{
	t_command		*f_command;
	t_pre_tokens	*node;
	t_pre_tokens	*temp_next;
	t_pre_tokens	*temp_to_next;

	if (!head || !(*head))
		return (0);
	f_command = 0;
	node = *head;
	while (node)
	{
		temp_next = add_to_command(node, &f_command);
		node = temp_next;
		while ((node) && (node->type == TYPE_RED_PIP))
		{
			temp_to_next = node->next;
			free(node->content);
			free(node);
			node = temp_to_next;
		}
	}
	return (f_command);
}
