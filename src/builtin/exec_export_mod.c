/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_export_mod.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 09:46:02 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/11 09:48:50 by bel-kdio         ###   ########.fr       */
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

void	if_there_is_index_and_value(t_env *env, t_env *exp, char *args, int j)
{
	char	*str;
	t_env	*new_node;

	while (args[j])
	{
		if (args[j] == '=')
			break ;
		j++;
	}
	if (!search_in_env(exp, ft_substr(args, 0, j)))
	{
		str = ft_substr(args, j + 1, ft_strlen(args));
		new_node = ft_lstnew_env(ft_substr(args, 0, j), str);
		ft_lstadd_back_env(&env, new_node);
		new_node = ft_lstnew_env(ft_substr(args, 0, j), str);
		ft_lstadd_back_env(&exp, new_node);
	}
	else
	{
		search_in_env_and_replace(exp, ft_substr(args, 0, j),
			ft_substr(args, j + 1, ft_strlen(args)));
		search_in_env_and_replace(env, ft_substr(args, 0, j),
			ft_substr(args, j + 1, ft_strlen(args)));
	}
}

void	mod_env_exp(t_env *env, t_env *exp, t_command *cmd)
{
	int			i;
	int			j;
	t_env		*new_node;
	char		*str;

	i = 0;
	while (cmd->db_args[i])
	{
		if (!ft_isalpha(cmd->db_args[i][0]))
			glob.exit_status = pr_err("minishell: export: ", cmd->db_args[i],
					": not a valid identifier\n", 1);
		j = 0;
		if (ft_strchr(cmd->db_args[i], '='))
			if_there_is_index_and_value(env, exp, cmd->db_args[i], 0);
		else
		{
			if (ft_isalpha(cmd->db_args[i][0]))
			{
				new_node = ft_lstnew_env(ft_substr(cmd->db_args[i], 0,
							ft_strlen(cmd->db_args[i])), NULL);
				ft_lstadd_back_env(&exp, new_node);
			}
		}
		i++;
	}
}
