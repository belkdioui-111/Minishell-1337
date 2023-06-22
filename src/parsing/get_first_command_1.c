/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_first_command_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabrahm <ylabrahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 19:19:44 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/06/21 19:22:55 by ylabrahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*ft_read_input(void)
{
	char	*us_in;
	char	*trimed_value;

	us_in = readline("minishell-1.0> ");
	if (!(us_in))
		ft_error(0);
	add_history(us_in);
	trimed_value = ft_strtrim(us_in, " \t");
	free(us_in);
	return (trimed_value);
}

int	sb_ad_ad(char *us_in, int start, int end, t_pre_tokens **head)
{
	char	*sub;
	int		ret;

	ret = 0;
	sub = ft_substr(us_in, start, end - start);
	ret = add_pre_t(head, sub, 0);
	free(sub);
	return (ret);
}

int	is_symbol(char symbol)
{
	if (symbol == '|' || symbol == '>' || symbol == '<')
		return (1);
	return (0);
}

int	ad_sbl(t_pre_tokens **head, char *us_in, int start, int *end)
{
	char	*symbole_to_add;
	int		ret;

	sb_ad_ad(us_in, start, *end, head);
	symbole_to_add = malloc(3);
	symbole_to_add[0] = us_in[*end];
	symbole_to_add[1] = '\0';
	if (us_in[*end + 1])
	{
		if (us_in[*end] == '>' && us_in[*end + 1] == '>')
		{
			symbole_to_add[1] = '>';
			symbole_to_add[2] = '\0';
			(*end)++;
		}
		if (us_in[*end] == '<' && us_in[*end + 1] == '<')
		{
			symbole_to_add[1] = '<';
			symbole_to_add[2] = '\0';
			(*end)++;
		}
	}
	ret = add_pre_t(head, symbole_to_add, 0);
	free(symbole_to_add);
	return (ret);
}

void	*ft_init_zeros(tokenizer_t *tok)
{
	tok->end = -1;
	tok->start = 0;
	tok->in_quotes = 0;
	tok->in_double_quotes = 0;
	return (0);
}
