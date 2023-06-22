/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 22:29:39 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/22 15:58:38 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	check_syntax(char *args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if ((args[i] >= 'a' && args[i] <= 'z') || (args[i] >= 'A'
				&& args[i] <= 'Z') || (args[i] >= '1' && args[i] <= '9')
			|| args[i] == '_')
		{
			i++;
			continue ;
		}
		else
		{
			g_glob.exit_status = pr_err("minishell: unset: `", args,
					"': not a valid identifier\n", 1);
			return (1);
		}
		i++;
	}
	return (0);
}

int	mod_env_and_exp(t_env *env_tmp, char *args, t_env *prev_env, t_env **env)
{
	if (check_syntax(args))
		return (1);
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
			return (0);
		}
		else
		{
			prev_env = env_tmp;
			env_tmp = env_tmp->next;
		}
	}
	return (0);
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
	if (mod_env_and_exp(env_tmp, args, prev_env, env))
	{
		return ;
	}
	mod_env_and_exp(export_tmp, args, prev_export, export);
}

int	exec_unset(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		check_and_free_unset(&g_glob.env, args[i], &g_glob.export);
		i++;
	}
	return (7);
}
