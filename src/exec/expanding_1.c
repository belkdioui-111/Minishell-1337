/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 15:03:00 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/20 15:03:33 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	get_len_if_sin_quo(int *i, int *len, char *var)
{
	while (var[*i] && var[*i] != '\'')
	{
		(*i)++;
		(*len)++;
	}
	(*i)++;
}

void	get_len_if_db_quo(int *i, int *len, char *var, t_env *env)
{
	char	*key;
	char	*value;

	while (var[*i] != '"')
	{
		if (var[*i] == '$' && ++(*i))
		{
			key = get_index(var + *i);
			value = get_value(key, &env);
			*i += ft_strlen(key);
			*len += ft_strlen(value);
			free(key);
			free(value);
		}
		else
		{
			(*i)++;
			(*len)++;
		}
	}
	(*i)++;
}

void	get_len_if_db_dollar(int *i, int *len, char *var, t_env *env)
{
	char	*key;
	char	*value;

	key = get_index(var + *i);
	value = get_value(key, &env);
	*i += ft_strlen(key);
	*len += ft_strlen(value);
	free(key);
	free(value);
}

int	get_len(char *var, t_env *env)
{
	int		len;
	int		i;
	char	*key;
	char	*value;

	len = 0;
	i = 0;
	while (var[i])
	{
		if (var[i] == '\'' && ++i)
			get_len_if_sin_quo(&i, &len, var);
		else if (var[i] == '"' && ++i)
			get_len_if_db_quo(&i, &len, var, env);
		else if (var[i] == '$' && ++i)
			get_len_if_db_dollar(&i, &len, var, env);
		else
		{
			i++;
			len++;
		}
	}
	return (len);
}
