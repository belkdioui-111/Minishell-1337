/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_if_buil.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 14:59:29 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/08 23:13:29 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	part_one_for_checking(char *s, int i)
{
	if (ft_strncmp(s, "cd", 3) == 0)
	{
		if (i == 1)
			return (1);
		return (11);
	}
	else if (ft_strncmp(s, "echo", 5) == 0)
	{
		if (i == 1)
			return (2);
		return (12);
	}
	else if (ft_strncmp(s, "pwd", 4) == 0)
	{
		if (i == 1)
			return (3);
		return (13);
	}
	return (0);
}

int	part_two_for_checking(char *s, int i)
{
	if (ft_strncmp(s, "export", 7) == 0)
	{
		if (i == 1)
			return (4);
		return (14);
	}
	else if (ft_strncmp(s, "exit", 5) == 0)
	{
		if (i == 1)
			return (5);
		return (15);
	}
	else if (ft_strncmp(s, "env", 4) == 0)
	{
		if (i == 1)
			return (6);
		return (16);
	}
	else if (ft_strncmp(s, "unset", 6) == 0)
	{
		if (i == 1)
			return (7);
		return (17);
	}
	return (0);
}

int	check_if_buil(char *s, t_command *cmds)
{
	int			i;
	t_command	*tmp;
	int			ret;

	i = 0;
	tmp = cmds;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	tmp = cmds;
	ret = part_one_for_checking(s, i);
	if (ret)
		return (ret);
	ret = part_two_for_checking(s, i);
	if (ret)
		return (ret);
	return (0);
}
