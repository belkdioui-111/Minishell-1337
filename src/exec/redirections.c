/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 10:07:17 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/22 15:17:43 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	redirection(t_command *head)
{
	t_pre_tokens	*out;
	t_pre_tokens	*in;
	int				cmdn;
	int				check;

	cmdn = 0;
	out = head->output_files;
	in = head->input_files;
	if (head->output_files)
		if (check_if_output(&cmdn, head->cmd, out))
			return (1);
	if (head->input_files)
	{
		check = check_if_input(&cmdn, head, in);
		if (check == 1)
			return (1);
		else if (check == 2)
			return (2);
	}
	else if (head->herdoc_files)
		if (check_if_heredocs(head) == 2)
			return (2);
	if (cmdn)
		return (2);
	return (0);
}
