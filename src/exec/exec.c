/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 14:21:18 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/05/31 21:41:45 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	**get_path(char **envp)
{
	int		i;
	char	**path;
	char	*trimmed;
	int		check;

	i = 0;
	check = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp("PATH", envp[i], 4) == 0)
		{
			check = 1;
			break ;
		}
		i++;
	}
	trimmed = ft_strtrim(envp[i], "PATH=");
	path = ft_split(trimmed, ':');
	return (path);
}

void set_path(t_command *head_command, char **env)
{
	t_command* tmp1;
	char **path;
	int i;
	
	tmp1 = head_command;
	while(tmp1)
	{
		path = get_path(env);
		i = 0;
		while (path[i])
		{
			path[i] = ft_strjoin(path[i], "/");
			path[i] = ft_strjoin(path[i], tmp1->cmd);
			if (access(path[i], F_OK | X_OK) != -1)
			{
				tmp1->path = path[i];
				break;
			}
			else
				tmp1->path = NULL;
			i++;
		}
		tmp1 = tmp1->next;
	}
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

int	calculate_number_of_args_in_node(t_command *all_cmd)
{
	int		i;
	int		j;
	char	**args;

	i = 1;
	j = 0;
	args = all_cmd->db_args;
	while (args[j])
	{
		j++;
	}
	return (j + i);
}

int	calculate_len_of_w(t_command *all_cmd, int i)
{
	char	*cmd;
	char	**args;
	int		len;

	len = 0;
	args = all_cmd->db_args;
	cmd = all_cmd->cmd;
	if (i == 0)
	{
		while (cmd[len])
		{
			len++;
		}
	}	
	else if (i > 0)
	{
		while (args[i - 1][len])
		{
			len++;
		}
	}
	return (len);
}

void	fill(t_command *all_cmd, int i, char *ev_word)
{
	char	*cmd;
	char	**args;
	int		j;

	j = 0;
	args = all_cmd->db_args;
	cmd = all_cmd->cmd;
	if (i == 0)
	{
		while (cmd[j])
		{
			ev_word[j] = cmd[j];
			j++;
		}
	}
	else if (i > 0)
	{
		i--;
		while (args[i][j])
		{
			ev_word[j] = args[i][j];
			j++;
		}
	}
}

void	fill_it(char **arr_of_ev_cmd, t_command *all_cmd)
{
	int	i;
	int	num_args_in_n;
	int	len_of_word;

	i = 0;
	num_args_in_n = calculate_number_of_args_in_node(all_cmd);
	while (i < num_args_in_n)
	{
		len_of_word = calculate_len_of_w(all_cmd, i);
		arr_of_ev_cmd[i] = malloc(sizeof(char) * (len_of_word + 1));
		arr_of_ev_cmd[i][len_of_word] = '\0';
		fill(all_cmd, i, arr_of_ev_cmd[i]);
		i++;
	}
}

void	fill_arr_of_all(char ***arr_of_all_cmd, t_command *all_cmd)
{
	int			i;
	int			num_of_cmd;
	int			num_args_in_n;
	t_command	*tmp;

	i = 0;
	tmp = all_cmd;
	num_of_cmd = calculate_num_of_cmd(all_cmd);
	while (i < num_of_cmd)
	{
		num_args_in_n = calculate_number_of_args_in_node(tmp);
		arr_of_all_cmd[i] = malloc(sizeof(char *) * (num_args_in_n + 1));
		fill_it(arr_of_all_cmd[i], tmp);
		arr_of_all_cmd[i][num_args_in_n] = NULL;
		tmp = tmp->next;
		i++;
	}
}

char	***convert_linked_list_to_tr_p(t_command *head_command)
{
	char	***arr_of_all;
	int		num_of_cmd;
	t_command *all_cmd;
	
	all_cmd = head_command;
	num_of_cmd = calculate_num_of_cmd(all_cmd);
	arr_of_all = malloc(sizeof(char **) * (num_of_cmd +1));
	if (!arr_of_all)
		return (0);
	arr_of_all[num_of_cmd] = NULL;
	fill_arr_of_all(arr_of_all, all_cmd);
	return (arr_of_all);
}







//execution

void	exec(char ***all_cmd, t_command *head, char **envp)
{
	int i;
	int *all_pid;
	// int pid;
	int status;
	int pipefd_next[2];
	int prev_pipe;

	all_pid=malloc(sizeof(int) * (calculate_num_of_cmd(head) + 1));
	all_pid[calculate_num_of_cmd(head)] = 0;
	i = 0;
	while(all_cmd[i] != NULL)
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
				dup2(pipefd_next[1],1);
				close(pipefd_next[0]);	
			}
			if (i != 0 && all_cmd[i + 1] != NULL)
			{
				dup2(prev_pipe, 0);
				dup2(pipefd_next[1],1);
				close(pipefd_next[0]);
			}
			if (all_cmd[i + 1] == NULL)
			{
				dup2(prev_pipe, 0);
			}
			execve(head->path, all_cmd[i],envp);
			perror(0);
			exit(0) ;
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
	while(all_pid[i])
	{
		waitpid(all_pid[i], &status, 0);
		i++;
	}
	// exit(status);
}