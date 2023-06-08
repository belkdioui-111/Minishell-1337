/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 22:23:14 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/08 23:20:43 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	check_numric(char *content)
{
	int	i;
	int	num;
	int	len;

	i = 0;
	len = ft_strlen(content);
	if (len == 0)
		return (1);
	else
	{
		if (content[i] && content[i] == '-')
		{
			i++;
			while (content[i])
			{
				if (ft_isdigit(content[i]) == 0)
					return (1);
				i++;
			}
			if (len == i)
				return (2);
		}
	}
	i = 0;
	while (content[i])
	{
		if (ft_isdigit(content[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	exec_exit(t_pre_tokens *args, int status)
{
	if (args)
	{
		if (args->next)
		{
			if (check_numric(args->content) == 1)
			{
				ft_putstr_fd("minishell: exit: ", 2);
				ft_putstr_fd(args->content, 2);
				ft_putstr_fd(": numeric argument required\n", 2);
				status = 255;
			}
			else if (check_numric(args->content) == 2)
			{
				exit((unsigned char)status);
			}
			else
			{
				ft_putstr_fd("minishell: exit: ", 2);
				ft_putstr_fd("too many arguments\n", 2);
				status = 1;
			}
		}
		else
		{
			if (check_numric(args->content) == 1)
			{
				ft_putstr_fd("minishell: exit: ", 2);
				ft_putstr_fd(args->content, 2);
				ft_putstr_fd(": numeric argument required\n", 2);
				status = 255;
			}
			else
				status = ft_atoi(args->content);
		}
		exit((unsigned char)status);
	}
	else
		exit(globals.exit_status);
	return (5);
}
