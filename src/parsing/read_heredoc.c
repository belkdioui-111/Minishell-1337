/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabrahm <ylabrahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 19:01:18 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/06/21 19:57:41 by ylabrahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	join_herdoc(char **herdoc, char **string, int contains_quotes,
		t_env *env_head)
{
	if (contains_quotes == 0)
		*string = expand_variable(*string, env_head, 2);
	*herdoc = ft_strjoin(*herdoc, *string);
	free(*string);
}

int	is_delimiter(char *del, char *content)
{
	int		check;
	char	*deli;

	deli = ft_strtrim(del, "\n");
	check = ft_strncmp(content, deli, ft_strlen(content) + 1);
	free(content);
	free(deli);
	if (check == 0)
		return (1);
	return (0);
}

int	ft_read_heredoc_while(char *string, t_pre_tokens **herdoc,
		t_command *command, t_env *env_head)
{
	int		cq;
	char	*yes;

	if (isatty(0))
		ft_putstr_fd("> ", 1);
	string = get_next_line(0);
	if (!string)
		return (1);
	yes = ft_strdup((*herdoc)->content);
	if (is_delimiter(string, remove_quote(yes)))
	{
		*herdoc = (*herdoc)->next;
		if (*herdoc)
			reset_here(&(command->here_doc_data));
		free(string);
	}
	else
	{
		cq = contains_quotes((*herdoc)->content);
		join_herdoc(&(command->here_doc_data), &string, cq, env_head);
	}
	return (0);
}

int	handle_gigs(void)
{
	if (glob.in_herdoc == 3)
	{
		glob.in_herdoc = 0;
		return (1);
	}
	return (0);
}

int	ft_read_heredoc(t_command **command_ix, t_env *env_head)
{
	t_command		*command;
	t_pre_tokens	*herdoc;
	char			*string;
	int				pipe_hd[2];

	command = *command_ix;
	herdoc = command->herdoc_files;
	if (herdoc == NULL)
		return (-1);
	command->here_doc_data = ft_calloc(1, 1);
	while (herdoc)
	{
		glob.in_herdoc = 1;
		if (ft_read_heredoc_while(string, &herdoc, command, env_head))
			break ;
	}
	if (handle_gigs())
		return (-2);
	if (pipe(pipe_hd) == -1)
		return (0);
	if (pipe_hd[1] != -1 && command->here_doc_data)
		write(pipe_hd[1], command->here_doc_data,
			ft_strlen(command->here_doc_data));
	close(pipe_hd[1]);
	return (pipe_hd[0]);
}
