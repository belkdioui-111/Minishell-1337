/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabrahm <ylabrahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 23:50:51 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/06/13 18:55:12 by ylabrahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	ft_set_cmd_type(char *command)
{
	if (!(command))
		return (-1);
	if ((ft_strncmp(command, "echo", 4) == 0)
		&& (ft_strlen(command) == ft_strlen("echo")))
		return (1);
	if ((ft_strncmp(command, "cd", 2) == 0)
		&& (ft_strlen(command) == ft_strlen("cd")))
		return (2);
	if ((ft_strncmp(command, "pwd", 3) == 0)
		&& (ft_strlen(command) == ft_strlen("pwd")))
		return (3);
	if ((ft_strncmp(command, "export", 6) == 0)
		&& (ft_strlen(command) == ft_strlen("export")))
		return (4);
	if ((ft_strncmp(command, "unset", 5) == 0)
		&& (ft_strlen(command) == ft_strlen("unset")))
		return (5);
	if ((ft_strncmp(command, "env", 3) == 0)
		&& (ft_strlen(command) == ft_strlen("env")))
		return (6);
	if ((ft_strncmp(command, "exit", 4) == 0)
		&& (ft_strlen(command) == ft_strlen("exit")))
		return (7);
	return (0);
}

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
			f_cmd->cmd = NULL;
			if (ft_strlen(f_cmd->args->content) != 0)
				f_cmd->cmd = ft_strdup(f_cmd->args->content);
			f_cmd->cmd_type = ft_set_cmd_type(f_cmd->cmd);
			free(f_cmd->args->content);
			free(f_cmd->args);
			f_cmd->args = temp_next;
		}
		else
			f_cmd->cmd = NULL;
		f_cmd = f_cmd->next;
	}
}
