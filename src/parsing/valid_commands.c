/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 04:26:41 by macbook           #+#    #+#             */
/*   Updated: 2023/06/06 11:31:15 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	add_returned_to_files(char *data, t_command **command_ix, int ret_type)
{
	t_command	*command;

	command = *command_ix;
	if (ret_type == TYPE_RED_OUT)
	{
		add_pre_t_2(&(command->output_files), data, NULL);
		command->out_type = 1;
	}
	if (ret_type == TYPE_RED_IN)
	{
		add_pre_t_2(&(command->input_files), data, NULL);
		command->in_type = 1;
	}
	if (ret_type == TYPE_RED_APP)
	{
		add_pre_t_2(&(command->append_files), data, NULL);
		command->out_type = 2;
	}
	if (ret_type == TYPE_RED_HER)
	{
		add_pre_t_2(&(command->herdoc_files), data, NULL);
		command->in_type = 2;
	}
	free(data);
}

int    check_redirections(t_command **command_ix)
{
    t_pre_tokens    *args;
    char            *returned;
    int                ret_type;

    args = (*command_ix)->args;
    while (args)
    {
        if (args->type != TYPE_ARG)
        {
            /* ERROR */
            if (!(args->next))
            {
                (*command_ix)->has_error = 1;
                return (1);
            }
            else
            {
                /* ERROR */
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
				add_returned_to_files(ft_strdup(node->content), commands_ix, node->prev->type);
				node = node->next;
				continue;
			}
			add_pre_t_2(&new_arguments, node->content, node);
		}
		node = node->next;
	}
	return (new_arguments);
}

int valid_commands(t_command **head_commands)
{
    int                ret;
    t_command        *command;
    t_pre_tokens    *temp;
    int                stpo = 0;

    ret = 0;
    command = *head_commands;
    while (command)
    {
        ret += check_redirections(&command);
        temp = command->args;
        command->args = ft_set_files(&command);
        free_linked(&temp);
        command = command->next;
    }
    if (ret != 0)
        print_error("parsing error\n");
    command = *head_commands;
    while (command)
    {
        if (command->has_error)
            stpo = 1;
        if (!stpo)
            ft_read_heredoc(&command);
        command = command->next;
    }
    return (ret > 0);
}
