/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_if_buil.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 14:59:29 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/21 17:43:06 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	part_one_for_checking(char *s)
{
	if (ft_strncmp(s, "cd", 3) == 0)
		return (1);
	else if (ft_strncmp(s, "echo", 5) == 0)
		return (2);
	else if (ft_strncmp(s, "pwd", 4) == 0)
		return (3);
	return (0);
}

int	part_two_for_checking(char *s)
{
	if (ft_strncmp(s, "export", 7) == 0)
		return (4);
	else if (ft_strncmp(s, "exit", 5) == 0)
		return (5);
	else if (ft_strncmp(s, "env", 4) == 0)
		return (6);
	else if (ft_strncmp(s, "unset", 6) == 0)
		return (7);
	return (0);
}

int	check_if_buil(char *s)
{
	int	ret;

	ret = part_one_for_checking(s);
	if (ret)
		return (ret);
	ret = part_two_for_checking(s);
	if (ret)
		return (ret);
	return (0);
}
