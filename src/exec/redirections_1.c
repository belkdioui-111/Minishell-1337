/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 14:54:31 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/20 14:59:38 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	output(t_pre_tokens *out, t_env *env)
{
	char	*exp;
	int		amb;
	int		fd;

	amb = 0;
	while (out)
	{
		exp = expand_redirs(out->content);
		if (exp == NULL)
			return (glob.exit_status = pr_err("minishell: ", NULL,
					"ambiguous redirect\n", 1));
		if (out->type == TYPE_RED_OUT)
			fd = open(exp, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (out->type == TYPE_RED_APP)
			fd = open(exp, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
			return (glob.exit_status = perr(exp, strerror(errno), 1), free(exp),
				1);
		dup2(fd, 1);
		close(fd);
		out = out->next;
		free(exp);
	}
	return (0);
}

int	input(t_pre_tokens *in, t_env *env)
{
	char	*exp;
	int		amb;
	int		fd;

	amb = 0;
	while (in)
	{
		exp = expand_redirs(in->content);
		if (exp == NULL)
			return (glob.exit_status = pr_err("minishell: ", NULL,
					"ambiguous redirect\n", 1));
		fd = open(exp, O_RDONLY);
		if (fd == -1)
			return (glob.exit_status = perr(exp, strerror(errno), 1), free(exp),
				1);
		dup2(fd, 0);
		close(fd);
		in = in->next;
	}
	return (0);
}

int	check_if_output(int *cmdn, char *cmd, t_pre_tokens *out)
{
	if (output(out, glob.env))
		return (1);
	if (cmd == NULL)
		*cmdn = 2;
	return (0);
}

int	check_if_input(int *cmdn, t_command *head, t_pre_tokens *in)
{
	if (input(in, glob.env))
		return (1);
	if (head->in_type == TYPE_RED_HER)
	{
		if (head->herdoc_files)
		{
			if (head->cmd == NULL)
				return (2);
			dup2(head->pipe_hd, 0);
			close(head->pipe_hd);
		}
	}
	if (head->cmd == NULL)
		*cmdn = 2;
	return (0);
}

int	check_if_heredocs(t_command *head)
{
	if (head->cmd == NULL)
		return (2);
	dup2(head->pipe_hd, 0);
	close(head->pipe_hd);
	return (0);
}
