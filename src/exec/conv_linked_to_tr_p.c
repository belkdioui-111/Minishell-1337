/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv_linked_to_tr_p.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 13:03:24 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/11 10:05:29 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
	char		***arr_of_all;
	int			num_of_cmd;
	t_command	*all_cmd;

	all_cmd = head_command;
	num_of_cmd = calculate_num_of_cmd(all_cmd);
	arr_of_all = malloc(sizeof(char **) * (num_of_cmd + 1));
	if (!arr_of_all)
		return (0);
	arr_of_all[num_of_cmd] = NULL;
	fill_arr_of_all(arr_of_all, all_cmd);
	return (arr_of_all);
}
