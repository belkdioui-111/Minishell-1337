/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 14:21:18 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/09 13:07:44 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
	globals.exit_status = 0;
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
			is_built = check_if_buil(head->cmd, head_command);
			if (is_built == 11 || is_built == 12 || is_built == 13
				|| is_built == 14 || is_built == 15 || is_built == 16
				|| is_built == 17)
			{
				exec_built(is_built, head, env, exp);
				exit(globals.exit_status);
			}
			else
			{
				if ((head->path) && (ft_strncmp(head->path, "cmdnull", 8) == 0))
				{
					globals.exit_status = 0;
					exit(globals.exit_status);
				}
				if ((head->path) && (ft_strncmp(head->path, "not", 4) == 0))
				{
					ft_putstr_fd("minishell: ", 2);
					ft_putstr_fd(all_cmd[i][0], 2);
					ft_putstr_fd(": No such file or directory\n", 2);
					globals.exit_status = 127;
					exit(globals.exit_status);
				}
				else if ((head->path) && (ft_strncmp(head->path, "dir",
							4) == 0))
				{
					ft_putstr_fd("minishell: ", 2);
					ft_putstr_fd(all_cmd[i][0], 2);
					ft_putstr_fd(": is a directory\n", 2);
					globals.exit_status = 126;
					exit(globals.exit_status);
				}
				else if (!head->path)
				{
					ft_putstr_fd("minishell: ", 2);
					ft_putstr_fd(all_cmd[i][0], 2);
					ft_putstr_fd(": command not found\n", 2);
					globals.exit_status = 127;
					exit(globals.exit_status);
				}
				execve(head->path, all_cmd[i], convert_link_to_2p(env));
				exit(globals.exit_status);
			}
		}
		else
		{
			if (all_cmd[i + 1] != NULL)
			{
				prev_pipe = pipefd_next[0];
				close(pipefd_next[1]);
			}
		}
		head = head->next;
		i++;
	}
	i = 0;
	while (all_pid[i])
	{
		waitpid(all_pid[i], &status, 0);
		i++;
	}
	globals.exit_status = status >> 8;
	// exit(status);
}
