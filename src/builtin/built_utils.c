/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 22:21:19 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/08 22:40:58 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	search_in_env_and_replace(t_env *env, char *index, char *str)
{
	t_env	*tmp;
	char	*newvalue;

	tmp = env;
	while (tmp)
	{
		if (ft_strncmp(index, tmp->index, ft_strlen(index)) == 0)
		{
			newvalue = ft_strdup(str);
			tmp->value = newvalue;
			return ;
		}
		tmp = tmp->next;
	}
}

char	*search_in_env(t_env *env, char *s)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strncmp(s, tmp->index, ft_strlen(s)) == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}
