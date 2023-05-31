/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabrahm <ylabrahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 16:13:40 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/05/31 01:02:04 by ylabrahm         ###   ########.fr       */
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
	new_command->args->prev = NULL;
	new_command->next = NULL;
	new_command->cmd = NULL;
	new_command->output_files = NULL;
	new_command->input_files = NULL;
	new_command->append_files = NULL;
	new_command->herdoc_files = NULL;
	while (temp_node->next && (ft_strncmp(temp_node->next->content, "|", ft_strlen(temp_node->next->content)) != 0))
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
	while (node && (ft_strncmp(node->content, "|", ft_strlen(node->content)) == 0))
		node = node->next;
	while (node)
	{
		temp_next = add_to_command(node, &f_command);
		node = temp_next;
		while (node && (ft_strncmp(node->content, "|", ft_strlen(node->content)) == 0))
		{
			temp_to_next = node->next;
			free(node->content);
			free(node);
			node = temp_to_next;
		}
	}
	return (f_command);
}
