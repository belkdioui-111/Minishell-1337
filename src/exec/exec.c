/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 14:21:18 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/19 17:34:21 by bel-kdio         ###   ########.fr       */
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
		glob.exit_status = pr_err("minishell: ", cmd,
				": command not found\n", 127);
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

void	simple_execute(char **cmd, int *pipes, int fd, t_command *node,
	t_command *head, t_env *env, t_env *exp)
{
	int		is_built;
	char	**e;

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
	is_built = check_if_buil(node->cmd, head);
	
	if (is_built >= 11 && is_built <= 17)
	{
		if (node->in_error == 0 || 1)
		{
			if (redirection(node, env) == 0 && redirection(node, env) != 2)
				exec_built(is_built, node, env, exp);
		}
		exit(glob.exit_status);
	}
	else
	{
		if (node->in_error == 0 || 1)
		{
			if (redirection(node, env) == 0 && redirection(node, env) != 2)
			{
				check_paths(node->path, cmd[0]);
				e = convert_link_to_2p(env);
				execve(node->path, cmd, e);
			}
		}
		exit(glob.exit_status);
	}
}

void	exec(char ***all_cmd, t_command *head, t_env *exp, t_env *env)
{
	int			count_cmds;
	int			is_built;
	int			pid;
	int			i;
	int			pipes[2];
	int			status;
	int			fd;
	int			tmp;
	t_command	*head_command;

	i = 0;
	head_command = head;
	count_cmds = calculate_num_of_cmd(head);
	if (count_cmds == 1)
	{
		head->path = set_path(head, env);
		is_built = check_if_buil(head->cmd, head);
		if (head && (is_built == 0 || (is_built >= 11 && is_built <= 17)))
		{
			pid = fork();
			if (pid == 0)
			{
				simple_execute(all_cmd[0], NULL, -1, head,
					head_command, env, exp);
			}
			else
			{
				waitpid(pid, &status, 0);
				glob.exit_status = status >> 8;
			}
		}
		else
		{
			if (head->in_error == 0 || 1)
			{
				int fdin;
				int fdout;
				fdin = dup(0);
				fdout = dup(1);
				if (redirection(head, env) == 0 && redirection(head, env) != 2)
					exec_built(is_built, head, env, exp);
				dup2(fdin, 0);
				close(fdin);
				dup2(fdout, 1);
				close(fdout);
			}
		}
	}
	else
	{
		fd = -1;
		while (all_cmd[i])
		{
			head->path = set_path(head, env);
			if (i != count_cmds - 1)
				pipe(pipes);
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
			pid = fork();
			if (pid == 0)
				simple_execute(all_cmd[i], pipes, fd, head,
					head_command, env, exp);
			close(pipes[1]);
			close(pipes[0]);
			head = head->next;
			i++;
		}
		close(fd);
		free_all_cmd(all_cmd);
		waitpid(pid, &status, 0);
		glob.exit_status = status >> 8;
		while (waitpid(-1, &status, 0) != -1)
		{
		}
	}
}
