/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_export_mod.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 09:46:02 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/22 16:05:43 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_env	*ft_lstlast_env(t_env *lst)
{
	t_env	*temp;

	if (!lst)
		return (NULL);
	temp = lst;
	while (temp->next != NULL)
	{
		temp = temp->next;
	}
	return (temp);
}

void	ft_lstadd_back_env(t_env **lst, t_env *new)
{
	t_env	*temp;

	if (*lst != NULL)
	{
		temp = ft_lstlast_env(*lst);
		temp->next = new;
	}
	else
		*lst = new;
}

t_env	*ft_lstnew_env(char *index, char *value)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->index = index;
	new_node->value = value;
	new_node->next = NULL;
	return (new_node);
}

void	if_there_is_an_equal(char *args, int j, char *str, char *index)
{
	t_env	*new_node;
	char	*tmp;

	if (!search_in_env(g_glob.export, index))
	{
		new_node = ft_lstnew_env(ft_strdup(index),
				ft_substr(args, j + 1, ft_strlen(args)));
		ft_lstadd_back_env(&g_glob.env, new_node);
		new_node = ft_lstnew_env(ft_strdup(index),
				ft_substr(args, j + 1, ft_strlen(args)));
		ft_lstadd_back_env(&g_glob.export, new_node);
	}
	else
	{
		tmp = ft_substr(args, j + 1, ft_strlen(args));
		search_in_env_and_replace(g_glob.export, index, tmp);
		if (!search_in_env(g_glob.env, index))
		{
			str = ft_substr(args, j + 1, ft_strlen(args));
			new_node = ft_lstnew_env(index, str);
			ft_lstadd_back_env(&g_glob.env, new_node);
		}
		search_in_env_and_replace(g_glob.env, index, tmp);
		free (tmp);
	}
}

void	if_there_is_index_and_value(char *args)
{
	t_env	*new_node;
	int		j;
	char	*index;

	new_node = NULL;
	j = loop_for_equal(args);
	if (check_syntax_export(args))
		return ;
	if (j)
		index = ft_substr(args, 0, j);
	else
		index = ft_strdup(args);
	if (j != 0)
		if_there_is_an_equal(args, j, NULL, index);
	else if (!j)
	{
		if (!search_in_env(g_glob.export, index))
		{
			new_node = ft_lstnew_env(ft_strdup(index), NULL);
			ft_lstadd_back_env(&g_glob.export, new_node);
		}
	}
	free(index);
}
