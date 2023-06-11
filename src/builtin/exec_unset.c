/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 22:29:39 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/11 08:47:57 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	mod_env_and_exp(t_env *env_tmp, char *args,
	t_env *prev_env, t_env **env)
{
	while (env_tmp)
	{
		if (ft_strncmp(env_tmp->index, args, ft_strlen(args)) == 0)
		{
			if (prev_env != NULL)
				prev_env->next = env_tmp->next;
			else
				*env = env_tmp->next;
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
}

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
	mod_env_and_exp(env_tmp, args, prev_env, env);
	mod_env_and_exp(export_tmp, args, prev_export, export);
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
