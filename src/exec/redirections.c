/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 10:07:17 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/11 12:28:42 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	output(t_pre_tokens *out)
{
	int	out_file;

	while (out)
	{
		out_file = open(out->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (out_file != -1)
		{
			dup2(out_file, STDOUT_FILENO);
			close(out_file);
		}
		out = out->next;
	}		
}

void	input(t_pre_tokens *in)
{
	int	in_file;

	while (in)
	{
		in_file = open(in->content, O_RDONLY);
		if (in_file != -1)
		{
			dup2(in_file, STDIN_FILENO);
			close(in_file);
		}
		in = in->next;
	}
}

void	redirection(t_command *head)
{
	t_pre_tokens	*out;
	t_pre_tokens	*in;

	out = head->output_files;
	in = head->input_files;
	if (head->output_files)
		output(out);
	if (head->input_files)
		input(in);
}
