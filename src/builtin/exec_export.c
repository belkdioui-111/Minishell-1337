/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 22:31:54 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/22 16:05:53 by bel-kdio         ###   ########.fr       */
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
			ft_putstr_fd("\\", 1);
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
			ft_putstr_fd("\\", 1);
		ft_putchar_fd(value[i], 1);
		if (value[i + 1] == '"' || value[i + 1] == '$')
			ft_putstr_fd("\\", 1);
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

void	mod_env_exp(t_command *cmd)
{
	int		i;

	i = 0;
	while (cmd->db_args[i])
	{
		if_there_is_index_and_value(cmd->db_args[i]);
		i++;
	}
}

int	exec_export(t_command *cmds)
{
	t_command	*cmd;

	cmd = cmds;
	if (!cmds->db_args[0])
		print_export(g_glob.export);
	else
		mod_env_exp(cmd);
	return (4);
}
