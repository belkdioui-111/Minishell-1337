/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_t.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabrahm <ylabrahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 16:01:42 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/06/21 19:32:51 by ylabrahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	add_to_last(t_pre_tokens **head, t_pre_tokens *next_node)
{
	t_pre_tokens	*node;
	t_pre_tokens	*prev;

	if (*head)
	{
		node = *head;
		while (node->next)
			node = node->next;
		prev = node;
		node->next = next_node;
		next_node->prev = prev;
	}
	else
	{
		*head = next_node;
	}
}

void	check_loop(t_check_arg *arg)
{
	while (arg->content[++arg->i])
	{
		if (arg->content[arg->i] == '\'')
		{
			if (!arg->d_quote)
			{
				if (arg->top >= 0 && arg->stack[arg->top] == '\'')
					arg->top--;
				else
					arg->stack[++arg->top] = '\'';
				arg->s_quote = !arg->s_quote;
			}
		}
		else if (arg->content[arg->i] == '\"')
		{
			if (!arg->s_quote)
			{
				if (arg->top >= 0 && arg->stack[arg->top] == '\"')
					arg->top--;
				else
					arg->stack[++arg->top] = '\"';
				arg->d_quote = !arg->d_quote;
			}
		}
	}
}

int	check_pre_t(char *content)
{
	t_check_arg	args;

	args.top = -1;
	args.s_quote = 0;
	args.d_quote = 0;
	args.i = -1;
	args.stack = malloc(ft_strlen(content) + 1);
	args.content = ft_strdup(content);
	check_loop(&args);
	free(args.stack);
	free(args.content);
	return (args.top < 0);
}

int	add_pre_t(t_pre_tokens **head, char *content, int state)
{
	t_pre_tokens	*created_node;

	if (!(content))
		return (1);
	created_node = malloc(sizeof(t_pre_tokens));
	if (!(created_node))
		return (1);
	if (state == 0)
	{
		if (!(check_pre_t(content)))
		{
			free(created_node);
			return (1);
		}
	}
	if (ft_strlen(content) != 0)
	{
		created_node->content = ft_strdup(content);
		created_node->next = NULL;
		created_node->prev = NULL;
		add_to_last(head, created_node);
	}
	else
		free(created_node);
	return (0);
}

int	add_pre_t_2(t_pre_tokens **head, char *content,
	t_pre_tokens *node, enum token_type type)
{
	t_pre_tokens	*created_node;

	if (!(content))
		return (1);
	created_node = malloc(sizeof(t_pre_tokens));
	if (!(created_node))
		return (1);
	if (ft_strlen(content) == 0)
	{
		if ((node) && (!(node->contain_quotes)))
		{
			free(created_node);
			return (1);
		}
		created_node->content = ft_strdup("");
	}
	else
		created_node->content = ft_strdup(content);
	created_node->next = NULL;
	created_node->prev = NULL;
	if (type)
		created_node->type = type;
	add_to_last(head, created_node);
	return (0);
}
