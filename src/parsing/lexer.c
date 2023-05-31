/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbook <macbook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 23:50:51 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/05/31 02:44:40 by macbook          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_lexer(t_command **head)
{
	t_command		*f_cmd;
	t_pre_tokens	*temp_next;

	f_cmd = *head;
	while (f_cmd)
	{
		if (f_cmd->args)
		{
			temp_next = f_cmd->args->next;
			f_cmd->cmd = ft_strdup(f_cmd->args->content);
			free(f_cmd->args->content);
			free(f_cmd->args);
			f_cmd->args = temp_next;
		}
		else
			f_cmd->cmd = NULL;
		f_cmd = f_cmd->next;
	}
}
