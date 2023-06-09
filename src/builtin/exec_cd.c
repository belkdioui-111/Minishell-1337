/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 22:19:47 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/09 16:11:32 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	exec_cd(t_command *cmd, t_env *env, t_env *export)
{
	if (access(cmd->db_args[0], F_OK) == 0)
	{
		search_in_env_and_replace(env, "OLDPWD", search_in_env(env, "PWD"));
		search_in_env_and_replace(export, "OLDPWD", search_in_env(export,
				"PWD"));
		chdir(cmd->db_args[0]);
		search_in_env_and_replace(env, "PWD", getcwd(NULL, 0));
		search_in_env_and_replace(export, "PWD", getcwd(NULL, 0));
	}
	else if (!cmd->db_args[0] || ft_strncmp(cmd->db_args[0], "~", 2) == 0)
	{
		search_in_env_and_replace(env, "OLDPWD", search_in_env(env, "PWD"));
		search_in_env_and_replace(export, "OLDPWD", search_in_env(export,
				"PWD"));
		chdir(search_in_env(env, "HOME"));
		search_in_env_and_replace(env, "PWD", getcwd(NULL, 0));
		search_in_env_and_replace(export, "PWD", getcwd(NULL, 0));
	}
	else
	{
		ft_putstr_fd("minishell: No such file or directory\n", 2);
		glob.exit_status = 1;
	}
	return (1);
}
