/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 14:50:34 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/22 16:01:05 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*expand_redirs(char *var)
{
	char	*exp;
	char	**arr;
	int		n;

	exp = expnd(var, &n);
	arr = ft_split(exp, 2);
	n = 0;
	while (arr[n])
		n++;
	if (n > 1)
		return (free_double(arr), free(exp), NULL);
	else if (n == 0 && (ft_strchr(exp, 2) || (!ft_strchr(var, '"')
				&& !ft_strchr(var, '\''))))
		return (free_double(arr), free(exp), NULL);
	free(exp);
	exp = arr[0];
	if (arr[0] == NULL)
		exp = ft_strdup("");
	free(arr);
	return (exp);
}

void	exp_if_sin_quo(int *i, int *j, char *var, char *exp)
{
	while (var[*i] && var[*i] != '\'')
	{
		exp[(*j)++] = var[(*i)++];
	}
	(*i)++;
}

void	exp_if_db_quo(int *i, int *j, char *var, char *exp)
{
	char	*key;
	char	*value;

	while (var[*i] != '"')
	{
		if (var[*i] == '$' && ++(*i))
		{
			if (is_valid_variable(var[*i]) == 0)
			{
				(*i)--;
				exp[(*j)++] = var[(*i)++];
			}
			key = get_index(var + *i);
			value = exit_st(key);
			*i += ft_strlen(key);
			ft_memcpy(&exp[*j], value, ft_strlen(value));
			*j += ft_strlen(value);
			free(key);
			free(value);
		}
		else
			exp[(*j)++] = var[(*i)++];
	}
	(*i)++;
}

void	exp_if_dollar(int *i, int *j, char *var, char *exp)
{
	char	*key;
	char	*value;
	int		k;

	if (is_valid_variable(var[*i]) == 0)
	{
		(*i)--;
		exp[(*j)++] = var[(*i)++];
	}
	else
	{
		key = get_index(var + *i);
		value = exit_st(key);
		*i += ft_strlen(key);
		k = -1;
		while (value[++k])
			if (value[k] == ' ' || value[k] == '\t')
				value[k] = 2;
		ft_memcpy(&exp[*j], value, ft_strlen(value));
		*j += ft_strlen(value);
		free(key);
		free(value);
	}
}

char	*expnd(char *var, int *amb)
{
	char	*exp;
	int		i;
	int		j;

	(void)amb;
	exp = ft_calloc(get_len(var) + 1, 1);
	i = 0;
	j = 0;
	while (var[i])
	{
		if (var[i] == '\'' && ++i)
			exp_if_sin_quo(&i, &j, var, exp);
		else if (var[i] == '"' && ++i)
			exp_if_db_quo(&i, &j, var, exp);
		else if (var[i] == '$' && ++i)
			exp_if_dollar(&i, &j, var, exp);
		else
			exp[j++] = var[i++];
	}
	return (exp);
}
