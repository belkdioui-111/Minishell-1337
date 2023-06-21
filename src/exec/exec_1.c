/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 16:44:11 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/20 16:46:08 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	exec_built_in_simple(int is_built, t_command *node)
{
	int	ret_red;

	ret_red = redirection(node);
	if (ret_red == 0 && ret_red != 2)
		exec_built(is_built, node);
	exit(glob.exit_status);
}

void	exec_cmd_in_simple(char **cmd, t_command *node)
{
	int		ret_red;
	char	**e;

	ret_red = redirection(node);
	if (ret_red == 0 && ret_red != 2)
	{
		check_paths(node->path, cmd[0]);
		e = convert_link_to_2p(glob.env);
		execve(node->path, cmd, e);
	}
	exit(glob.exit_status);
}

void	simple_execute(char **cmd, int *pipes, int fd, t_command *node)
{
	int		is_built;
	char	**e;
	int		ret_red;

	if (fd > -1)
		close(fd);
	if (pipes)
	{
		if (pipes[0] >= 0)
			dup2(pipes[0], 0);
		if (pipes[1] >= 0)
			dup2(pipes[1], 1);
		close(pipes[0]);
		close(pipes[1]);
	}
	is_built = check_if_buil(node->cmd);
	if (is_built >= 1 && is_built <= 7)
		exec_built_in_simple(is_built, node);
	else
		exec_cmd_in_simple(cmd, node);
}
