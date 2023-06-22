/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 13:15:58 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/22 12:41:13 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	perr(char *arg, char *msg, int ret)
{
	ft_putstr_fd("minishell: ", 2);
	if (arg)
		ft_putstr_fd(arg, 2);
	if (arg)
		ft_putstr_fd(": ", 2);
	ft_putendl_fd(msg, 2);
	return (ret);
}

int	calculate_num_of_cmd(t_command *all_cmd)
{
	int	num_of_cmd;

	num_of_cmd = 0;
	while (all_cmd)
	{
		num_of_cmd++;
		all_cmd = all_cmd->next;
	}
	return (num_of_cmd);
}

void	free_all_cmd(char ***all_cmd)
{
	int	i;
	int	j;

	i = 0;
	while (all_cmd[i] != NULL)
	{
		j = 0;
		while (all_cmd[i][j] != NULL)
		{
			free(all_cmd[i][j]);
			j++;
		}
		free(all_cmd[i]);
		i++;
	}
	free(all_cmd);
}

char	*exit_st(char *key)
{
	char	*value;

	if (ft_strncmp(key, "?", 2) == 0)
		value = ft_itoa(g_glob.exit_status);
	else
		value = get_value(key, &g_glob.env);
	return (value);
}

void	check_paths(char *path, char *cmd)
{
	if ((path) && (ft_strncmp(path, "cmdnull", 8) == 0))
	{
		g_glob.exit_status = pr_err("minishell: ", cmd, ": command not found\n",
				127);
		exit(g_glob.exit_status);
	}
	if ((path) && (ft_strncmp(path, "not", 4) == 0))
	{
		g_glob.exit_status = pr_err("minishell: ", cmd,
				": No such file or directory\n", 127);
		exit(g_glob.exit_status);
	}
	else if ((path) && (ft_strncmp(path, "dir", 4) == 0))
	{
		g_glob.exit_status = pr_err("minishell: ", cmd, ": is a directory\n",
				126);
		exit(g_glob.exit_status);
	}
	else if (!path)
	{
		g_glob.exit_status = pr_err("minishell: ", cmd, ": command not found\n",
				127);
		exit(g_glob.exit_status);
	}
}
