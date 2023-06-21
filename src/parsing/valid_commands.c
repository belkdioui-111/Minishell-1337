/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabrahm <ylabrahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 04:26:41 by macbook           #+#    #+#             */
/*   Updated: 2023/06/21 10:37:01 by ylabrahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	add_returned_to_files(char *data, t_command **command_ix, int ret_type)
{
	t_command	*command;

	command = *command_ix;
	if ((ret_type == TYPE_RED_OUT) || (ret_type == TYPE_RED_APP))
	{
		add_pre_t_2(&(command->output_files), data, NULL, ret_type);
		command->out_type = 1;
	}
	if (ret_type == TYPE_RED_IN)
	{
		add_pre_t_2(&(command->input_files), data, NULL, 0);
		command->in_type = TYPE_RED_IN;
	}
	if (ret_type == TYPE_RED_HER)
	{
		add_pre_t_2(&(command->herdoc_files), data, NULL, 0);
		command->in_type = TYPE_RED_HER;
	}
	free(data);
}

int	check_redirections(t_command **command_ix)
{
	t_pre_tokens	*args;

	args = (*command_ix)->args;
	while (args)
	{
		if (args->type != TYPE_ARG)
		{
			if (!(args->next))
			{
				(*command_ix)->has_error = 1;
				return (1);
			}
			else
			{
				if (args->next->type != TYPE_ARG)
				{
					(*command_ix)->has_error = 1;
					return (1);
				}
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
				add_returned_to_files(ft_strdup(node->content), commands_ix,
					node->prev->type);
				node = node->next;
				continue ;
			}
			add_pre_t_2(&new_arguments, node->content, node, 0);
		}
		node = node->next;
	}
	return (new_arguments);
}

int	check_in_error(t_command **commands_ix, t_env *env_head)
{
	t_command		*command;
	t_pre_tokens	*node;
	int				ambiguous;
	int				ret;

	command = *commands_ix;
	node = command->args;
	ambiguous = 0;
	while (node)
	{
		if ((!node->prev) || ((node->prev) && (node->prev->type == TYPE_ARG
					|| node->prev->type == TYPE_RED_PIP)))
			node->content = remove_quote(node->content);
		node = node->next;
	}
	return (0);
}

int	valid_commands_2(t_command **head_commands, int ret, t_env *env_head)
{
	t_command	*command;
	int			stpo;

	stpo = 0;
	command = *head_commands;
	while (command)
	{
		if (command->has_error)
			stpo = 1;
		if (!stpo)
		{
			command->pipe_hd = ft_read_heredoc(&command, env_head);
			if (command->pipe_hd == -2)
				return (1);
		}
		command = command->next;
	}
	return (0);
}

int	valid_commands(t_command **head_commands, t_env *env_head)
{
	int				ret;
	t_command		*command;
	t_pre_tokens	*temp;
	int				stpo;

	stpo = 0;
	ret = 0;
	command = *head_commands;
	while (command)
	{
		ret += check_redirections(&command);
		temp = command->args;
		// command->in_error = check_in_error(&command, env_head);
		command->args = ft_set_files(&command);
		free_linked(&temp);
		command = command->next;
	}
	if (ret != 0)
		print_error("syntax error\n", 258);
	if (valid_commands_2(head_commands, ret, env_head))
		return (1);
	return (ret > 0);
}
