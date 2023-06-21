/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 14:21:18 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/20 19:10:35 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	if_onecmd_built(t_command *head, int is_built)
{
	int	fdin;
	int	fdout;
	int	ret_red;

	fdin = dup(0);
	fdout = dup(1);
	ret_red = redirection(head);
	if (ret_red == 0 && ret_red != 2)
		exec_built(is_built, head);
	dup2(fdin, 0);
	close(fdin);
	dup2(fdout, 1);
	close(fdout);
}

void	if_onecmd_not_built(char **all_cmd, t_command *head)
{
	int	pid;
	int	status;

	pid = fork();
	if (pid == 0)
		simple_execute(all_cmd, NULL, -1, head);
	else
	{
		waitpid(pid, &status, 0);
		glob.exit_status = status >> 8;
	}
}

int	handling_pipes(int i, int fd, int count_cmds, int *pipes)
{
	int	tmp;

	if (i == 0)
	{
		fd = pipes[0];
		pipes[0] = -1;
	}
	else if (i < count_cmds - 1)
	{
		tmp = pipes[0];
		pipes[0] = fd;
		fd = tmp;
	}
	else
	{
		pipes[0] = fd;
		pipes[1] = -1;
		fd = -1;
	}
	return (fd);
}

int	if_mult_cmds(t_command *head, int count_cmds, char ***all_cmd)
{
	int	pid;
	int	i;
	int	pipes[2];
	int	status;
	int	fd;

	fd = -1;
	i = -1;
	while (all_cmd[++i])
	{
		head->path = set_path(head);
		if (i != count_cmds - 1)
			pipe(pipes);
		fd = handling_pipes(i, fd, count_cmds, pipes);
		pid = fork();
		if (pid == 0)
			simple_execute(all_cmd[i], pipes, fd, head);
		close(pipes[1]);
		close(pipes[0]);
		head = head->next;
	}
	close(fd);
	free_all_cmd(all_cmd);
	waitpid(pid, &status, 0);
	return (status);
}

void	exec(char ***all_cmd, t_command *head)
{
	int	count_cmds;
	int	is_built;
	int	status;

	count_cmds = calculate_num_of_cmd(head);
	if (count_cmds == 1)
	{
		head->path = set_path(head);
		is_built = check_if_buil(head->cmd);
		if (head && is_built == 0)
			if_onecmd_not_built(all_cmd[0], head);
		else
			if_onecmd_built(head, is_built);
	}
	else
	{
		status = if_mult_cmds(head, count_cmds, all_cmd);
		glob.exit_status = status >> 8;
		while (waitpid(-1, &status, 0) != -1)
		{
		}
	}
}
