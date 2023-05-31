/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbook <macbook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 04:26:41 by macbook           #+#    #+#             */
/*   Updated: 2023/05/31 02:37:21 by macbook          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	add_returned_to_files(char *data, t_command **command_ix, int ret_type)
{
	t_command	*command;

	command = *command_ix;
	/* TYPE_RED_OUT */
	if (ret_type == TYPE_RED_OUT)
		add_pre_t(&(command->output_files), data);
	/* TYPE_RED_IN */
	if (ret_type == TYPE_RED_IN)
		add_pre_t(&(command->input_files), data);
	/* TYPE_RED_APP */
	if (ret_type == TYPE_RED_APP)
		add_pre_t(&(command->append_files), data);
	/* TYPE_RED_HER */
	if (ret_type == TYPE_RED_HER)
		add_pre_t(&(command->herdoc_files), data);
	free(data);
}

int	check_redirections(t_command **command_ix)
{
	t_pre_tokens	*args;
	char			*returned;
	int				ret_type;

	args = (*command_ix)->args;
	while (args)
	{
		if (args->type != TYPE_ARG)
		{
			/* ERROR */
			if (!(args->next))
				return (1);
			else
			{
				/* ERROR */
				if (args->next->type != TYPE_ARG)
					return (1);
			}
		}
		args = args->next;
	}
	return (0);
}

t_pre_tokens	*ft_set_files(t_command **commands_ix)
{
	t_pre_tokens	*node;
	t_pre_tokens	*temp;
	t_pre_tokens	*new_arguments;

	new_arguments = NULL;
	node = (*commands_ix)->args;
	while (node)
	{
		if (node->type == TYPE_ARG)
		{
			if ((node->prev) && (node->prev->type != TYPE_ARG))
			{
				add_returned_to_files(ft_strdup(node->content), commands_ix, node->prev->type);
				node = node->next;
				continue;
			}
			add_pre_t(&new_arguments, node->content);
		}
		node = node->next;
	}
	return (new_arguments);
}

int valid_commands(t_command **head_commands)
{
	int			ret;
	t_command	*command;

	ret = 0;
	command = *head_commands;
	while (command)
	{
		ret += check_redirections(&command);
		if (ret != 0)
		{
			print_error("parsing error\n");
			return (1);
		}
		command = command->next;
	}
	command = *head_commands;
	while (command)
	{
		t_pre_tokens	*temp;

		temp = command->args;
		command->args = ft_set_files(&command);
		free_linked(&temp);
		command = command->next;
	}
	return (0);
}
