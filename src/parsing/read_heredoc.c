/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabrahm <ylabrahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 19:01:18 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/06/05 11:32:47 by ylabrahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_delimiter(char *del, char *content)
{

	if (ft_strncmp(content, del, (ft_strlen(content) + 1)) == 0)
		return (1);
	return (0);
}

void	ft_read_heredoc(t_command **command_ix)
{
	t_command		*command;
	t_pre_tokens	*herdoc;
	char			*string;
	char			*all;

	command = *command_ix;
	string = 0;
	herdoc = command->herdoc_files;
	if (herdoc)
		command->here_doc_data = ft_calloc(1, 1);
	while (herdoc)
	{
		string = readline("> ");
		if (is_delimiter(string, herdoc->content))
			herdoc = herdoc->next;
		if (herdoc)
		{
			command->here_doc_data = ft_strjoin(command->here_doc_data, string);
			command->here_doc_data = ft_strjoin(command->here_doc_data, "\n");
		}
		free(string);
	}
}
