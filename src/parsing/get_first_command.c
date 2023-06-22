/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_first_command.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabrahm <ylabrahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 16:43:08 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/06/21 19:24:50 by ylabrahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	ft_tokenizer_loop(tokenizer_t *tok)
{
	int	ret;

	ret = 0;
	while (tok->us_in[++tok->end] != '\0')
	{
		if (tok->us_in[tok->end] == '\"' && !(tok->in_quotes))
			tok->in_double_quotes = !(tok->in_double_quotes);
		if (tok->us_in[tok->end] == '\'' && !(tok->in_double_quotes))
			tok->in_quotes = !(tok->in_quotes);
		if (!(tok->in_quotes) && !(tok->in_double_quotes))
		{
			if (tok->us_in[tok->end] == ' ')
			{
				ret += sb_ad_ad(tok->us_in, tok->start, tok->end, &tok->head);
				tok->start = tok->end + 1;
			}
			if (is_symbol(tok->us_in[tok->end]))
			{
				ret += ad_sbl(&tok->head, tok->us_in, tok->start, &tok->end);
				tok->start = tok->end + 1;
			}
		}
	}
	ret += sb_ad_ad(tok->us_in, tok->start, tok->end, &tok->head);
	return (ret);
}

t_pre_tokens	*ft_tokenizer(char *us_in)
{
	tokenizer_t	tok;
	char		*error;

	tok.head = ft_init_zeros(&tok);
	tok.us_in = ft_strdup(us_in);
	if (ft_tokenizer_loop(&tok) != 0)
	{
		free_linked(&(tok.head));
		print_error("unexpected EOF while looking for matching\n", 258);
		free(tok.us_in);
		return (0);
	}
	free(tok.us_in);
	return (tok.head);
}

void	free_sub(t_pre_tokens *args)
{
	int	i;

	i = 0;
	while ((args->sub.sub)[i])
	{
		if ((args->sub.sub)[i] != ((char *)-1))
			free((args->sub.sub)[i]);
		i++;
	}
	free((args->sub.sub));
}

t_pre_tokens	*ft_set_subs(t_pre_tokens **args)
{
	t_pre_tokens	*node;
	t_pre_tokens	*returned;
	int				i;

	returned = NULL;
	node = *args;
	while (node)
	{
		i = 0;
		while ((node->sub.sub)[i])
		{
			if ((node->sub.sub)[i] == ((char *)-1))
				add_pre_t_2(&returned, "", 0, node->sub.type);
			else
				add_pre_t_2(&returned, (node->sub.sub)[i], 0, node->sub.type);
			i++;
		}
		free_sub(node);
		node = node->next;
	}
	free_linked(args);
	return (returned);
}

t_command	*get_first_command(char *us_in, t_env *env_head)
{
	t_pre_tokens	*head_args;
	t_command		*head_command;
	char			*error;

	head_args = ft_tokenizer(us_in);
	ft_remove_quotes(&head_args, env_head);
	head_args = ft_set_subs(&head_args);
	if (valid_arguments(&head_args) == 1)
		return (NULL);
	head_command = ft_fill_commands(&head_args);
	if (head_command)
	{
		if (valid_commands(&head_command, env_head) == 1)
		{
			free_commands(&head_command);
			return (NULL);
		}
		ft_lexer(&head_command);
	}
	return (head_command);
}
