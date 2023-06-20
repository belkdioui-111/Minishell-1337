/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabrahm <ylabrahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 19:01:18 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/06/20 11:30:43 by ylabrahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void reset_here(char **herdoc)
{
	free(*herdoc);
	*herdoc = ft_calloc(1, 1);
}

void join_herdoc(char **herdoc, char *string, int contains_quotes, t_env *env_head)
{
	if (contains_quotes == 0)
		string = expand_variable(ft_strdup(string), env_head, 2);
	*herdoc = ft_strjoin(*herdoc, string);
}

int is_delimiter(char *del, char *content)
{
	int check;
	char *deli;

	int i;
	deli = ft_strtrim(del, "\n");

	check = ft_strncmp(content, deli, ft_strlen(content)+1);
	if (check == 0)
		return (1);
	return (0);
}

int	ft_read_heredoc_while(char **string_ix, t_pre_tokens **herdoc, t_command *command, t_env *env_head)
{
	char	*string;

	string = *string_ix;
	if (isatty(0))
		ft_putstr_fd("> ", 1);
	string = get_next_line(0);
	if (!string)
		return (1);
	ft_strtrim(string, "\n");
	if (is_delimiter(string, remove_quote(ft_strdup((*herdoc)->content))))
	{
		*herdoc = (*herdoc)->next;
		if (*herdoc)
			reset_here(&(command->here_doc_data));
	}
	else
	{
		int	cq;
		cq = contains_quotes(ft_strdup((*herdoc)->content));
		join_herdoc(&command->here_doc_data, string, cq, env_head);
	}
	free(string);
	return (0);
}

int ft_read_heredoc(t_command **command_ix, t_env *env_head)
{
	t_command		*command;
	t_pre_tokens	*herdoc;
	char			*string;
	char			*all;
	int				pipe_hd[2];
	// t_pre_tokens	*temp;

	command = *command_ix;
	string = 0;
	// herdoc = NULL;
	// temp = command->herdoc_files;
	// while (temp)
	// {
	// 	if (temp->type == TYPE_RED_HER)
	// 		add_pre_t_2()
	// }
	herdoc = command->herdoc_files;
	if (herdoc)
		command->here_doc_data = ft_calloc(1, 1);
	while (herdoc)
		if (ft_read_heredoc_while(&string, &herdoc, command, env_head))
			break ;
	if (pipe(pipe_hd) == -1)
		return (0);
	if (pipe_hd[1] != -1 && command->here_doc_data)
		write(pipe_hd[1], command->here_doc_data, ft_strlen(command->here_doc_data));
	close(pipe_hd[1]);
	return (pipe_hd[0]);
}
