/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_arguments.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabrahm <ylabrahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 18:41:41 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/06/15 16:47:31 by ylabrahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	check_double_pips(t_pre_tokens **head_args)
{
	t_pre_tokens	*arg;

	arg = *head_args;
	if (arg->type == TYPE_RED_PIP)
		return (1);
	while (arg)
	{
		if (arg->type == TYPE_RED_PIP)
		{
			if (!(arg->next))
				return (1);
			if (arg->next->type == TYPE_RED_PIP)
				return (1);
		}
		arg = arg->next;
	}
	return (0);
}

int	valid_arguments(t_pre_tokens **head_args)
{
	int	ret;

	if ((!head_args) || (!(*head_args)))
		return (0);
	ret = check_double_pips(head_args);
	if (ret == 1)
	{
		print_error("syntax error\n", 258);
		free_linked(head_args);
	}
	return (ret);
}
