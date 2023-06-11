/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 22:31:54 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/11 09:53:48 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	print_index(char *index)
{
	int	i;

	i = 0;
	while (index[i])
	{
		printf("%c", index[i]);
		if (index[i + 1] == '"' || index[i + 1] == '$')
			printf("\\");
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
			printf("\\");
		printf("%c", value[i]);
		if (value[i + 1] == '"' || value[i + 1] == '$')
			printf("\\");
		i++;
	}
}

void	print_export(t_env *exp)
{
	while (exp)
	{
		printf("declare -x ");
		print_index(exp->index);
		if (!exp->value)
			printf("\n");
		if (exp->value)
		{
			printf("=");
			printf("\"");
			print_value(exp->value);
			printf("\"\n");
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
