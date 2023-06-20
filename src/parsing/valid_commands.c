/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabrahm <ylabrahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 04:26:41 by macbook           #+#    #+#             */
/*   Updated: 2023/06/20 11:31:37 by ylabrahm         ###   ########.fr       */
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
	if (ret_type == TYPE_RED_IN/* || ret_type == TYPE_RED_HER*/)
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

int    check_redirections(t_command **command_ix)
{
    t_pre_tokens    *args;

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
				add_returned_to_files(ft_strdup(node->content), commands_ix, node->prev->type);
				node = node->next;
				continue;
			}
			add_pre_t_2(&new_arguments, node->content, node, 0);
		}
		node = node->next;
	}
	return (new_arguments);
}

int		num_of_strs(char **strings)
{
	int	i;

	i = 0;
	if (!strings)
		return (0);
	while (strings[i])
		i++;
	return (i);
}

char	*expand_red(t_pre_tokens *node, int *ambiguous, t_env *env_head)
{
	t_sub	strings;
	int		total;
	int		i;

	strings = expand_variable_2(&node, env_head);
	total = num_of_strs(strings.sub);
	if ((total == 0 || total > 1))
	{
		*ambiguous = 1;
		free(node->content);
		return ft_strdup("");
	}
	// free(node->content);
	return (strings.sub[0]);
}

int	check_in_err_help(t_pre_tokens *node, int *ambiguous, t_env *env_head)
{
	int	in_file_fd;

	if (node->prev->type == TYPE_RED_IN)
	{
		node->content = expand_red(node, ambiguous, env_head);
		if (*ambiguous == 1)
			return (1);
		in_file_fd = open(node->content, O_RDONLY);
		if (in_file_fd == -1)
		{
			close(in_file_fd);
			return (2);
		}
		close(in_file_fd);
	}
	if (node->prev->type != TYPE_RED_HER)
	{
		if ((node->prev->type != TYPE_ARG) && (node->prev->type != TYPE_RED_PIP))
			node->content = expand_red(node, ambiguous, env_head);
		if (*ambiguous == 1)
			return (1);
	}
	return (0);
}

int check_in_error(t_command **commands_ix, t_env *env_head)
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
		// node->contain_quotes = contains_quotes(node->content);
		// if (node->prev)
		// {
		// 	ret = check_in_err_help(node, &ambiguous, env_head);
		// 	if (ret == 1 || ret == 2)
		// 		return (ret);
		// }
		if ((!node->prev) || ((node->prev) && (node->prev->type == TYPE_ARG || node->prev->type == TYPE_RED_PIP)))
			node->content = remove_quote(node->content);
		node = node->next;
	}
	return (0);
}

void	valid_commands_2(t_command **head_commands, int ret, t_env *env_head)
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
			command->pipe_hd = ft_read_heredoc(&command, env_head);
        command = command->next;
    }
    // if (ret == 0)
    // {
    //     command = *head_commands;
    //     while (command)
    //     {
    //         if (command->in_error == 1)
	// 			print_error("ambiguous redirect\n", 1);
    //         if (command->in_error == 2)
    //             print_error("No such file or directory\n", 1);
    //         command = command->next;
    //     }
    // }	
}

int valid_commands(t_command **head_commands, t_env *env_head)
{
    int             ret;
    t_command       *command;
    t_pre_tokens    *temp;
    int             stpo = 0;

    ret = 0;
    command = *head_commands;
    while (command)
    {
        ret += check_redirections(&command);
        temp = command->args;
        command->in_error = check_in_error(&command, env_head);
        command->args = ft_set_files(&command);
        free_linked(&temp);
        command = command->next;
    }
    if (ret != 0)
        print_error("syntax error\n", 258);
	valid_commands_2(head_commands, ret, env_head);
    return (ret > 0);
}
