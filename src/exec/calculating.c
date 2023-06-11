/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculating.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 10:04:20 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/11 10:06:00 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
		while (args[i - 1] && args[i - 1][len])
		{
			len++;
		}
	}
	return (len);
}
