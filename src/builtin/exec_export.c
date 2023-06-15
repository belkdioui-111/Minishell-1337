/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 22:31:54 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/15 13:18:37 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	print_index(char *index)
{
	int	i;

	i = 0;
	while (index[i])
	{
		ft_putchar_fd(index[i], 1);
		if (index[i + 1] == '"' || index[i + 1] == '$')
			ft_putstr_fd("\\",1);
		i++;
	}
}

void	print_value(char *value)
{
	int	i;

	i = 0;
	while (value[i])
	{
		if (i == 0 && (value[i] == '"' || value[i] == '$'))
			ft_putstr_fd("\\",1);
		ft_putchar_fd(value[i], 1);
		if (value[i + 1] == '"' || value[i + 1] == '$')
			ft_putstr_fd("\\",1);
		i++;
	}
}

void	print_export(t_env *exp)
{
	while (exp)
	{
		ft_putstr_fd("declare -x ", 1);
		print_index(exp->index);
		if (!exp->value)
			ft_putstr_fd("\n", 1);
		if (exp->value)
		{
			ft_putstr_fd("=", 1);
			ft_putstr_fd("\"", 1);
			print_value(exp->value);
			ft_putstr_fd("\"\n", 1);
		}
		exp = exp->next;
	}
}

int	exec_export(t_env *export, t_command *cmds, t_env *envi)
{
	t_env		*exp;
	t_env		*env;
	t_command	*cmd;

	exp = export;
	cmd = cmds;
	env = envi;
	if (!cmds->db_args[0])
		print_export(exp);
	else
		mod_env_exp(env, exp, cmd);
	return (4);
}
