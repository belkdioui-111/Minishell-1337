/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv_linked_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 13:10:33 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/09 13:21:54 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	ft_get_num_of_args(t_pre_tokens **arg_ix)
{
	int				i;
	t_pre_tokens	*arg;

	arg = *arg_ix;
	i = 0;
	while (arg)
	{
		i++;
		arg = arg->next;
	}
	return (i);
}

void	set_db_args(t_command **command_ix)
{
	t_command		*command;
	t_pre_tokens	*node;
	int				num_of_args;
	int				i;

	i = 0;
	command = *command_ix;
	num_of_args = ft_get_num_of_args(&(command->args));
	command->db_args = (char **)ft_calloc(num_of_args + 1, sizeof(char *));
	node = command->args;
	while (node)
	{
		command->db_args[i] = ft_strdup(node->content);
		node = node->next;
		i++;
	}
}

void	conver_l_args_to_p(t_command *head_command)
{
	t_command	*command;

	command = head_command;
	while (command)
	{
		set_db_args(&command);
		command = command->next;
	}
}

char	**convert_link_to_2p(t_env *env)
{
	t_env	*tmp;
	int		size;
	char	**arr;

	tmp = env;
	size = 0;
	while (tmp)
	{
		size++;
		tmp = tmp->next;
	}
	tmp = env;
	arr = malloc(sizeof(char *) * (size + 1));
	arr[size] = NULL;
	size = 0;
	while (tmp)
	{
		arr[size] = ft_strjoin(tmp->index, "=");
		arr[size] = ft_strjoin(arr[size], tmp->value);
		size++;
		tmp = tmp->next;
	}
	return (arr);
}
