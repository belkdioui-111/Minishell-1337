/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 13:15:58 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/21 14:54:03 by bel-kdio         ###   ########.fr       */
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

void	check_paths(char *path, char *cmd)
{
	if ((path) && (ft_strncmp(path, "cmdnull", 8) == 0))
	{
		glob.exit_status = pr_err("minishell: ", cmd, ": command not found\n",
				127);
		exit(glob.exit_status);
	}
	if ((path) && (ft_strncmp(path, "not", 4) == 0))
	{
		glob.exit_status = pr_err("minishell: ", cmd,
				": No such file or directory\n", 127);
		exit(glob.exit_status);
	}
	else if ((path) && (ft_strncmp(path, "dir", 4) == 0))
	{
		glob.exit_status = pr_err("minishell: ", cmd, ": is a directory\n",
				126);
		exit(glob.exit_status);
	}
	else if (!path)
	{
		glob.exit_status = pr_err("minishell: ", cmd, ": command not found\n",
				127);
		exit(glob.exit_status);
	}
}
