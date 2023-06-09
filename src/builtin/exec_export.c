/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 22:31:54 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/09 16:11:51 by bel-kdio         ###   ########.fr       */
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

int	exec_export(t_env *export, t_command *cmds, t_env *env)
{
	t_env		*exp;
	t_command	*cmd;
	int			i;
	int			j;
	t_env		*new_node;
	char		*str;

	exp = export;
	cmd = cmds;
	i = 0;
	if (!cmds->db_args[0])
	{
		while (exp)
		{
			printf("declare -x ");
			while (exp->index[i])
			{
				printf("%c", exp->index[i]);
				if (exp->index[i + 1] == '"' || exp->index[i + 1] == '$')
				{
					printf("\\");
				}
				i++;
			}
			if (!exp->value)
				printf("\n");
			if (exp->value)
			{
				printf("=");
				printf("\"");
				i = 0;
				while (exp->value[i])
				{
					if (i == 0 && (exp->value[i] == '"'
							|| exp->value[i] == '$'))
					{
						printf("\\");
					}
					printf("%c", exp->value[i]);
					if (exp->value[i + 1] == '"' || exp->value[i + 1] == '$')
					{
						printf("\\");
					}
					i++;
				}
				printf("\"\n");
			}
			i = 0;
			exp = exp->next;
		}
	}
	else
	{
		while (cmd->db_args[i])
		{
			if (!ft_isalpha(cmd->db_args[i][0]))
			{
				ft_putstr_fd("minishell: export: ", 2);
				ft_putstr_fd(cmd->db_args[i], 2);
				ft_putstr_fd(": not a valid identifier\n", 2);
				glob.exit_status = 1;
			}
			j = 0;
			if (ft_strchr(cmd->db_args[i], '='))
			{
				while (cmd->db_args[i][j])
				{
					if (cmd->db_args[i][j] == '=')
					{
						break ;
					}
					j++;
				}
				if (!search_in_env(exp, ft_substr(cmd->db_args[i], 0, j)))
				{
					str = ft_substr(cmd->db_args[i], j + 1,
							ft_strlen(cmd->db_args[i]));
					new_node = ft_lstnew_env(ft_substr(cmd->db_args[i], 0, j),
							str);
					ft_lstadd_back_env(&env, new_node);
					new_node = ft_lstnew_env(ft_substr(cmd->db_args[i], 0, j),
							str);
					ft_lstadd_back_env(&exp, new_node);
				}
				else
				{
					search_in_env_and_replace(exp, ft_substr(cmd->db_args[i], 0,
							j), ft_substr(cmd->db_args[i], j + 1,
							ft_strlen(cmd->db_args[i])));
					search_in_env_and_replace(env, ft_substr(cmd->db_args[i], 0,
							j), ft_substr(cmd->db_args[i], j + 1,
							ft_strlen(cmd->db_args[i])));
				}
			}
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
	return (4);
}
