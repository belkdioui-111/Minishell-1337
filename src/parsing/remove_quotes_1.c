/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabrahm <ylabrahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 20:11:37 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/06/21 20:22:24 by ylabrahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*get_index(char *string)
{
	int	i;

	i = 0;
	if ((string[i]) && ((string[i] == '?') || (string[i] == '@')))
		return (ft_substr(string, 0, i + 1));
	if ((string[i]) && (ft_isdigit(string[i])))
		return (ft_substr(string, 0, i + 1));
	while (1)
	{
		if (!(string[i] && (ft_isalnum(string[i]) || string[i] == '_')))
			break ;
		i++;
	}
	return (ft_substr(string, 0, i));
}

char	*get_value(char *index, t_env **head_env)
{
	t_env	*node;
	int		cmp;
	int		nlen;
	int		ilen;

	node = *head_env;
	while (node)
	{
		cmp = ft_strncmp(index, node->index, ft_strlen(node->index));
		ilen = ft_strlen(index);
		if (cmp == 0 && (ilen == ft_strlen(node->index)))
			return (ft_strdup(node->value));
		node = node->next;
	}
	return (ft_strdup(""));
}

void	four_free(char **tk, char **sf, char **evix, char **evvl)
{
	if (*tk)
		free(*tk);
	if (*sf)
		free(*sf);
	if (*evix)
		free(*evix);
	if (*evvl)
		free(*evvl);
}

char	*get_new_token(char **token, char *new, t_env *head_env, int i)
{
	char	*env_index;
	int		len_to;
	char	*env_value;
	char	*suffix;

	env_index = get_index(&(*token)[i + 1]);
	if (env_index[0] == '?')
		env_value = ft_itoa(glob.exit_status);
	else
		env_value = get_value(env_index, &head_env);
	new = ft_substr((*token), 0, i);
	new = ft_strjoin(new, env_value);
	len_to = ft_strlen(&(*token)[i + 1]) - ft_strlen(env_index);
	suffix = ft_substr(&(*token)[i + 1], ft_strlen(env_index), len_to);
	new = ft_strjoin(new, suffix);
	four_free(token, &suffix, &env_index, &env_value);
	return (new);
}

int	is_valid_variable(char after_dollar)
{
	return ((ft_isalnum(after_dollar)) || (after_dollar == '_')
		|| (after_dollar == '?') || (after_dollar == '@'));
}
