/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 22:19:47 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/22 12:52:34 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*search_in_env_and_return_value(t_env *env, char *s)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strncmp(s, tmp->index, ft_strlen(s) + 1) == 0)
		{
			return (tmp->value);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

void	if_cmd_null_or_home(void)
{
	char	*str;

	str = NULL;
	if (search_in_env(g_glob.env, "HOME"))
	{
		search_in_env_and_replace(g_glob.env, "OLDPWD",
			search_in_env_and_return_value(g_glob.env, "PWD"));
		search_in_env_and_replace(g_glob.export, "OLDPWD",
			search_in_env_and_return_value(g_glob.export, "PWD"));
		chdir(search_in_env_and_return_value(g_glob.env, "HOME"));
		str = getcwd(NULL, 0);
		search_in_env_and_replace(g_glob.env, "PWD", str);
		search_in_env_and_replace(g_glob.export, "PWD", str);
		free (str);
	}
	else
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
}

int	exec_cd(t_command *cmd)
{
	char	*str;

	str = NULL;
	if (access(cmd->db_args[0], F_OK) == 0)
	{
		search_in_env_and_replace(g_glob.env, "OLDPWD",
			search_in_env_and_return_value(g_glob.env, "PWD"));
		search_in_env_and_replace(g_glob.export, "OLDPWD",
			search_in_env_and_return_value(g_glob.export, "PWD"));
		chdir(cmd->db_args[0]);
		str = getcwd(NULL, 0);
		search_in_env_and_replace(g_glob.env, "PWD", str);
		search_in_env_and_replace(g_glob.export, "PWD", str);
		free (str);
	}
	else if (!cmd->db_args[0] || ft_strncmp(cmd->db_args[0], "~", 2) == 0)
	{
		if_cmd_null_or_home();
	}
	else
	{
		ft_putstr_fd("minishell: No such file or directory\n", 2);
		g_glob.exit_status = 1;
	}
	return (1);
}
