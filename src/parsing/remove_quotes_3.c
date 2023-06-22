/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabrahm <ylabrahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 20:15:01 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/06/21 21:55:44 by ylabrahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	set_up_remove_vars(int *j, int *in_single, int *in_double)
{
	(*j) = 0;
	(*in_single) = 0;
	(*in_double) = 0;
	return (0);
}

int	contains_quotes(char *content)
{
	int	i;

	i = 0;
	if (!content)
		return (0);
	while (content[i])
	{
		if (content[i] == '\'' || content[i] == '\"')
			return (1);
		i++;
	}
	return (0);
}

void	set_node_type(t_pre_tokens **head, int contain_quotes)
{
	int				i;
	t_pre_tokens	*node;

	i = 0;
	node = *head;
	node->type = TYPE_ARG;
	if (contain_quotes == 0)
	{
		if (ft_strlen(node->content) == 1)
		{
			if (ft_strncmp(node->content, "<", 1) == 0)
				node->type = TYPE_RED_IN;
			if (ft_strncmp(node->content, ">", 1) == 0)
				node->type = TYPE_RED_OUT;
			if (ft_strncmp(node->content, "|", 1) == 0)
				node->type = TYPE_RED_PIP;
		}
		else
		{
			if (ft_strncmp(node->content, ">>", 2) == 0)
				node->type = TYPE_RED_APP;
			if (ft_strncmp(node->content, "<<", 2) == 0)
				node->type = TYPE_RED_HER;
		}
	}
}

char	*remove_quote(char *content)
{
	char	*copy;
	int		i;
	int		j;
	int		in_single;
	int		in_double;

	i = set_up_remove_vars(&j, &in_single, &in_double);
	copy = malloc(ft_strlen(content) + 1);
	while (content[i])
	{
		if (content[i] == '\'' && !(in_double))
			in_single = !(in_single);
		else if (content[i] == '\"' && !(in_single))
			in_double = !(in_double);
		else
			copy[j++] = content[i];
		i++;
	}
	copy[j] = '\0';
	free(content);
	return (copy);
}

t_sub	get_sub_from_node(t_pre_tokens **node_ix)
{
	char	**sub;
	t_sub	returned;

	returned.type = (*node_ix)->type;
	returned.sub = ft_split((*node_ix)->content, 30);
	return (returned);
}
