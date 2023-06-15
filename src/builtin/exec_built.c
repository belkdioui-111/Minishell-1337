/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 22:17:50 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/14 22:06:14 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	exec_built(int n, t_command *cmds, t_env *env, t_env *export)
{
	if (n == 1 || n == 11)
		return (exec_cd(cmds, env, export));
	else if (n == 2 || n == 12)
		return (exec_echo(cmds));
	else if (n == 6 || n == 16)
		return (exec_env(env));
	else if (n == 5 || n == 15)
		return (exec_exit(cmds->args, 0));
	else if (n == 4 || n == 14)
		return (exec_export(export, cmds, env));
	else if (n == 3 || n == 13)
		return (exec_pwd(env));
	else if (n == 7 || n == 17)
		return (exec_unset(env, cmds->db_args, export));
	return (0);
}
