/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 13:15:58 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/09 13:23:45 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
