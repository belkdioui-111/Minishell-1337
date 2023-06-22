/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 01:03:21 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/06/22 12:08:05 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*ft_get_index(char *env_one)
{
	int		i;
	char	*ret;

	i = 0;
	while (env_one[i] && env_one[i] != '=')
		i++;
	ret = ft_substr(env_one, 0, i);
	return (ret);
}

char	*ft_get_value(char *env_one)
{
	int		i;
	int		j;
	char	*ret;

	i = 0;
	j = 0;
	while (env_one[i] && env_one[i] != '=')
		i++;
	i++;
	while (env_one[j])
		j++;
	ret = ft_substr(env_one, i, j);
	return (ret);
}

void	add_env_list(t_env **env_head, char *env_one)
{
	t_env	*new_env;
	t_env	*tmp_env;

	new_env = malloc(sizeof(t_env));
	new_env->next = NULL;
	new_env->index = ft_get_index(env_one);
	new_env->value = ft_get_value(env_one);
	if (!(*env_head))
		*env_head = new_env;
	else
	{
		tmp_env = *env_head;
		while (tmp_env->next)
			tmp_env = tmp_env->next;
		tmp_env->next = new_env;
	}
}

t_env	*ft_set_env(char **env)
{
	t_env	*env_head;
	int		i;

	i = 0;
	env_head = NULL;
	while (env[i])
	{
		add_env_list(&env_head, env[i]);
		i++;
	}
	return (env_head);
}
