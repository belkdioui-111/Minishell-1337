/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabrahm <ylabrahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 15:22:46 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/06/21 20:17:08 by ylabrahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_remove_quotes_2(t_pre_tokens **head, t_env *head_env)
{
	t_pre_tokens	*node;

	node = *head;
	while (node)
	{
		node->contain_quotes = contains_quotes(node->content);
		if (node->type == TYPE_ARG)
		{
			if ((!(node->prev)) || (node->prev->type == TYPE_ARG
					|| node->prev->type == TYPE_RED_PIP))
				node->sub = expand_variable_2(node, head_env);
			else
				node->sub = get_sub_from_node(&node);
		}
		else
			node->sub = get_sub_from_node(&node);
		node = node->next;
	}
}

void	ft_remove_quotes(t_pre_tokens **head, t_env *head_env)
{
	t_pre_tokens	*node;

	node = *head;
	while (node)
	{
		set_node_type(&node, contains_quotes(node->content));
		node->contain_quotes = contains_quotes(node->content);
		if (!(node->prev))
			node->content = expand_variable(node->content, head_env, 1);
		else if (node->prev->type == TYPE_ARG)
			node->content = expand_variable(node->content, head_env, 1);
		node = node->next;
	}
	ft_remove_quotes_2(head, head_env);
}
