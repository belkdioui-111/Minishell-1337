/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 14:21:18 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/07 17:34:13 by bel-kdio         ###   ########.fr       */
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
	if(!trimmed)
		return (NULL);
	path = ft_split(trimmed, ':');
	return (path);
}

void set_path(t_command *head_command, t_env *env_head)
{
	t_command* tmp1;
	char **path;
	int i;
	struct stat fileStat;
	
	tmp1 = head_command;
	while(tmp1)
	{
		if(!tmp1->cmd)
		{
			tmp1->path=ft_strdup("cmdnull");
			tmp1 = tmp1->next;
			continue;
		}
		if(check_if_buil(tmp1->cmd, tmp1) == 0)
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
           				tmp1->path= ft_strdup("dir");
        			}
				}
			}
			else
			{
				if(tmp1->cmd && ft_strchr(tmp1->cmd, '/'))
				{
					tmp1->path=ft_strdup("not");
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
							break;
						}
						else
							tmp1->path = NULL;
						i++;
					}
					if(!path)
						tmp1->path = NULL;
				}
			}
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
		while (cmd && cmd[len])
		{
			len++;
		}
	}	
	else if (i > 0)
	{
		while (args[i -1] && args[i - 1][len])
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
		while (cmd && cmd[j])
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





char 	**convert_link_to_2p(t_env *env)
{
	t_env *tmp;
	int size;
	char **arr;
	
	tmp = env;
	size = 0;
	while (tmp)
	{
		size++;
		tmp = tmp->next;
	}
	tmp = env;
	arr = malloc(sizeof(char *) * (size +1));
	arr[size] = NULL;
	size = 0;
	while (tmp)
	{
		arr[size] = ft_strjoin(tmp->index,"=");
		arr[size] = ft_strjoin(arr[size],tmp->value);
		size++;
		tmp = tmp->next;
	}
	return (arr);
}

//execution

void	exec(char ***all_cmd, t_command *head, t_env *exp, t_env *env)
{
	int i;
	int *all_pid;
	int status;
	int pipefd_next[2];
	int is_built;
	int prev_pipe;
	t_command *head_command;

	head_command = head;
	globals.exit_status = 0;
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

			is_built = check_if_buil(head->cmd, head_command);
			if(is_built == 11 || is_built == 12 || is_built == 13 || is_built == 14 || is_built == 15 || is_built == 16 || is_built == 17)
			{
				exec_built(is_built, head, env, exp);
				exit(globals.exit_status);
			}
			else
			{
				if((head->path) && (ft_strncmp(head->path,"cmdnull", 8) == 0))
				{
					globals.exit_status = 0;			
					exit(globals.exit_status);
				}
				if((head->path) && (ft_strncmp(head->path,"not", 4) == 0))
				{
					ft_putstr_fd("minishell: ", 2);
					ft_putstr_fd(all_cmd[i][0], 2);
					ft_putstr_fd(": No such file or directory\n", 2);
					globals.exit_status = 127;			
					exit(globals.exit_status);
				}
				else if((head->path) && (ft_strncmp(head->path,"dir", 4) == 0))
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
				execve(head->path, all_cmd[i],convert_link_to_2p(env));
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
	while(all_pid[i])
	{
		waitpid(all_pid[i], &status, 0);
		i++;
	}
	globals.exit_status = status >> 8;
	// exit(status);
}
