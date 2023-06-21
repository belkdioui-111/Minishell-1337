/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_export_mod.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 09:46:02 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/21 14:48:49 by bel-kdio         ###   ########.fr       */
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

void	if_there_is_index_and_value(char *args)
{
	char	*str;
	t_env	*new_node;
	int		j;

	j = loop_for_equal(args);
	if (check_syntax_export(args))
		return ;
	if (!search_in_env(glob.export, ft_substr(args, 0, j)))
	{
		str = ft_substr(args, j + 1, ft_strlen(args));
		new_node = ft_lstnew_env(ft_substr(args, 0, j), str);
		ft_lstadd_back_env(&glob.env, new_node);
		new_node = ft_lstnew_env(ft_substr(args, 0, j), str);
		ft_lstadd_back_env(&glob.export, new_node);
	}
	else
	{
		search_in_env_and_replace(glob.export, ft_substr(args, 0, j),
			ft_substr(args, j + 1, ft_strlen(args)));
		search_in_env_and_replace(glob.env, ft_substr(args, 0, j),
			ft_substr(args, j + 1, ft_strlen(args)));
	}
}

void	mod_env_exp(t_command *cmd)
{
	int		i;
	t_env	*new_node;
	char	*str;

	i = 0;
	while (cmd->db_args[i])
	{
		if (ft_strchr(cmd->db_args[i], '='))
			if_there_is_index_and_value(cmd->db_args[i]);
		else
		{
			if (check_syntax_export(cmd->db_args[i]))
			{
				i++;
				continue ;
			}
			new_node = ft_lstnew_env(ft_substr(cmd->db_args[i], 0,
						ft_strlen(cmd->db_args[i])), NULL);
			ft_lstadd_back_env(&glob.export, new_node);
		}
		i++;
	}
}
