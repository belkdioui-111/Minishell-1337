/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 22:23:14 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/22 15:58:19 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	check_neg(char *content)
{
	int	len;
	int	i;

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
	return (0);
}

int	check_numric(char *content)
{
	int	i;
	int	ret_check_neg;

	i = 0;
	ret_check_neg = check_neg(content);
	if (ret_check_neg)
		return (ret_check_neg);
	while (content[i])
	{
		if (ft_isdigit(content[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	if_args_exist(char *args_content, int status)
{
	if (check_numric(args_content) == 1)
		status = pr_err("minishell: exit: ", args_content,
				": numeric argument required\n", 255);
	else if (check_numric(args_content) == 2)
		exit((unsigned char)status);
	else
		status = pr_err("minishell: exit: ", "too many arguments\n", NULL, 1);
	return (status);
}

int	exec_exit(t_pre_tokens *args, int status)
{
	if (args)
	{
		if (args->next)
			status = if_args_exist(args->content, status);
		else
		{
			if (check_numric(args->content) == 1)
				status = pr_err("minishell: exit: ", args->content,
						": numeric argument required\n", 255);
			else
				status = ft_atoi(args->content);
		}
		exit((unsigned char)status);
	}
	else
		exit(g_glob.exit_status);
	return (5);
}
