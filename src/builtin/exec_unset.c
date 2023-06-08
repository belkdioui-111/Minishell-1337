/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 22:29:39 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/08 23:22:59 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	check_and_free_unset(t_env **env, char *args, t_env **export)
{
	t_env	*env_tmp;
	t_env	*prev_env;
	t_env	*export_tmp;
	t_env	*prev_export;

	env_tmp = *env;
	prev_env = NULL;
	export_tmp = *export;
	prev_export = NULL;
	while (env_tmp)
	{
		if (ft_strncmp(env_tmp->index, args, ft_strlen(args)) == 0)
		{
			if (prev_env != NULL)
				prev_env->next = env_tmp->next;
			else
				*env = env_tmp->next;
			free(env_tmp->index);
			free(env_tmp->value);
			free(env_tmp);
			if (prev_env != NULL)
				env_tmp = prev_env->next;
			else
				env_tmp = *env;
		}
		else
		{
			prev_env = env_tmp;
			env_tmp = env_tmp->next;
		}
	}
	while (export_tmp)
	{
		if (ft_strncmp(export_tmp->index, args, ft_strlen(args)) == 0)
		{
			if (prev_export != NULL)
				prev_export->next = export_tmp->next;
			else
				*export = export_tmp->next;
			free(export_tmp->index);
			free(export_tmp->value);
			free(export_tmp);
			if (prev_export != NULL)
				export_tmp = prev_export->next;
			else
				export_tmp = *export;
			return ;
		}
		else
		{
			prev_export = export_tmp;
			export_tmp = export_tmp->next;
		}
	}
}

int	exec_unset(t_env *env, char **args, t_env *export)
{
	int	i;

	i = 0;
	while (args[i])
	{
		check_and_free_unset(&env, args[i], &export);
		i++;
	}
	return (7);
}
