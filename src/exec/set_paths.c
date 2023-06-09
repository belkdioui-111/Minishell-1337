/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_paths.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:57:16 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/09 12:59:33 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	**get_path(t_env *envp)
{
	int		i;
	char	**path;
	char	*trimmed;
	int		check;

	i = 0;
	check = 0;
	trimmed = search_in_env(envp, "PATH");
	if (!trimmed)
		return (NULL);
	path = ft_split(trimmed, ':');
	return (path);
}

void	set_path(t_command *head_command, t_env *env_head)
{
	t_command	*tmp1;
	char		**path;
	int			i;
	struct stat	fileStat;

	tmp1 = head_command;
	while (tmp1)
	{
		if (!tmp1->cmd)
		{
			tmp1->path = ft_strdup("cmdnull");
			tmp1 = tmp1->next;
			continue ;
		}
		if (check_if_buil(tmp1->cmd, tmp1) == 0)
		{
			path = get_path(env_head);
			i = 0;
			if (access(tmp1->cmd, F_OK | X_OK) != -1)
			{
				if (stat(tmp1->cmd, &fileStat) == 0)
				{
					if (!(fileStat.st_mode & S_IFDIR))
					{
						tmp1->path = tmp1->cmd;
					}
					else
					{
						tmp1->path = ft_strdup("dir");
					}
				}
			}
			else
			{
				if (tmp1->cmd && ft_strchr(tmp1->cmd, '/'))
				{
					tmp1->path = ft_strdup("not");
				}
				else
				{
					while (path && path[i])
					{
						path[i] = ft_strjoin(path[i], "/");
						path[i] = ft_strjoin(path[i], tmp1->cmd);
						if (access(path[i], F_OK | X_OK) != -1)
						{
							tmp1->path = path[i];
							break ;
						}
						else
							tmp1->path = NULL;
						i++;
					}
					if (!path)
						tmp1->path = ft_strdup("not");
				}
			}
		}
		tmp1 = tmp1->next;
	}
}