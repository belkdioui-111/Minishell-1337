/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabrahm <ylabrahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 20:12:44 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/06/21 21:52:01 by ylabrahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	expand_loop_1(char **token, int in_s_q,
	int in_d_q, int i)
{
	t_pre_tokens	*head;
	char			*new;

	new = NULL;
	while ((*token)[++i])
	{
		if (((*token)[i] == '\'') && (!in_d_q))
			in_s_q = (!in_s_q);
		if (((*token)[i] == '\"') && (!in_s_q))
			in_d_q = (!in_d_q);
		if (((*token)[i] == '$') && (in_d_q) && (!in_s_q))
		{
			if (!((*token)[i + 1]))
				return ;
			new = get_new_token(token, new, glob.env, i);
			if ((ft_strlen(new) == 0))
			{
				*token = ft_strdup("");
				return ;
			}
			(*token) = ft_strdup(new);
			free(new);
		}
	}
}

void	expand_loop_2(char **token, int in_s_q, int in_d_q, int i)
{
	t_pre_tokens	*head;
	char			*new;

	new = NULL;
	while ((*token)[++i])
	{
		if (((*token)[i] == '\'') && (!in_d_q))
			in_s_q = (!in_s_q);
		if (((*token)[i] == '\"') && (!in_s_q))
			in_d_q = (!in_d_q);
		if ((*token)[i] == '$' && (!in_s_q))
		{
			if (!((*token)[i + 1]))
				return ;
			new = get_new_token(token, new, glob.env, i);
			if ((ft_strlen(new) == 0))
			{
				*token = ft_strdup("");
				return ;
			}
			(*token) = ft_strdup(new);
			free(new);
		}
	}
}

char	*expand_variable(char *token, t_env *head_env, int state)
{
	int	in_s_q;
	int	in_d_q;
	int	i;

	i = -1;
	in_s_q = 0;
	in_d_q = 0;
	if (state == 1)
		expand_loop_1(&token, in_s_q, in_d_q, i);
	else
		expand_loop_2(&token, in_s_q, in_d_q, i);
	return (token);
}

char	**expand_redirs_2(char *var, t_env *env)
{
	char	*exp;
	char	**arr;
	int		n;

	exp = expnd(var, &n);
	arr = ft_split(exp, 2);
	free(exp);
	if (!arr[0] && (ft_strchr(var, '\"') || ft_strchr(var, '\'')))
	{
		free(arr);
		arr = ft_calloc(2, sizeof(char *));
		arr[0] = (char *)-1;
		arr[1] = 0;
	}
	return (arr);
}

t_sub	expand_variable_2(t_pre_tokens *node, t_env *head_env)
{
	t_sub	ret;
	int		i;

	ret.sub = expand_redirs_2(node->content, head_env);
	ret.type = TYPE_ARG;
	i = -1;
	return (ret);
}
