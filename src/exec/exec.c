/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 14:21:18 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/11 10:08:15 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
		glob.exit_status = 0;
		exit(glob.exit_status);
	}
	if ((path) && (ft_strncmp(path, "not", 4) == 0))
	{
		glob.exit_status = pr_err("minishell: ", cmd,
				": No such file or directory\n", 127);
		exit(glob.exit_status);
	}
	else if ((path) && (ft_strncmp(path, "dir",
				4) == 0))
	{
		glob.exit_status = pr_err("minishell: ", cmd, ": is a directory\n", 126);
		exit(glob.exit_status);
	}
	else if (!path)
	{
		glob.exit_status = pr_err("minishell: ", cmd,
				": command not found\n", 127);
		exit(glob.exit_status);
	}
}

void	exec(char ***all_cmd, t_command *head, t_env *exp, t_env *env)
{
	int			i;
	int			*all_pid;
	int			status;
	int			pipefd_next[2];
	int			is_built;
	int			prev_pipe;
	t_command	*head_command;

	head_command = head;
	prev_pipe = -1;
	glob.exit_status = 0;
	all_pid = malloc(sizeof(int) * (calculate_num_of_cmd(head) + 1));
	all_pid[calculate_num_of_cmd(head)] = 0;
	i = 0;
	while (all_cmd[i] != NULL)
	{
		if (all_cmd[i + 1] != NULL)
			pipe(pipefd_next);
		all_pid[i] = fork();
		if (all_pid[i] == -1)
			return ;
		else if (all_pid[i] == 0)
		{
			head->path = set_path(head, env);
			if (i == 0)
			{
				dup2(pipefd_next[1], 1);
				close(pipefd_next[0]);
			}
			if (i != 0 && all_cmd[i + 1] != NULL)
			{
				dup2(prev_pipe, 0);
				dup2(pipefd_next[1], 1);
				close(pipefd_next[0]);
			}
			if (all_cmd[i + 1] == NULL)
			{
				dup2(prev_pipe, 0);
			}
			redirection(head);
			is_built = check_if_buil(head->cmd, head_command);
			if (is_built >= 11 && is_built <= 17)
			{
				exec_built(is_built, head, env, exp);
				exit(glob.exit_status);
			}
			else
			{
				check_paths(head->path, all_cmd[i][0]);
				execve(head->path, all_cmd[i], convert_link_to_2p(env));
				exit(glob.exit_status);
			}
		}
		else
		{
			close(prev_pipe);
			if (all_cmd[i + 1] != NULL)
			{
				prev_pipe = pipefd_next[0];
				close(pipefd_next[1]);
			}
		}
		head = head->next;
		i++;
	}
	free_all_cmd(all_cmd);
	i = 0;
	while (all_pid[i])
	{
		waitpid(all_pid[i], &status, 0);
		i++;
	}
	free(all_pid);
	glob.exit_status = status >> 8;
}
