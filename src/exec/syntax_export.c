/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_export.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 14:48:38 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/21 14:58:34 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	loop_for_equal(char *args)
{
	int	j;

	j = 0;
	while (args[j])
	{
		if (args[j] == '=')
			break ;
		j++;
	}
	return (j);
}

int	syntax_with_equal(char *args)
{
	int	j;

	j = 0;
	if (ft_isalpha(args[0]) || args[0] == '_')
	{
		j++;
		while (args[j])
		{
			if (args[j] == '=')
				break ;
			else if ((!(args[j] >= '1' && args[j] <= '9')) && (!(args[j] >= 'a'
						&& args[j] <= 'z')) && (!(args[j] >= 'A'
						&& args[j] <= 'Z')) && args[j] != '_')
				return (1);
			j++;
		}
	}
	else
		return (1);
	return (0);
}

int	syntax_without_equal(char *args)
{
	int	j;

	j = 0;
	if (ft_isalpha(args[0]) || args[0] == '_')
	{
		j++;
		while (args[j])
		{
			if ((!ft_isdigit(args[j])) && (!(args[j] >= 'a' && args[j] <= 'z'))
				&& (!(args[j] >= 'A' && args[j] <= 'Z')) && args[j] != '_')
				return (1);
			j++;
		}
	}
	else
		return (1);
	return (0);
}

int	check_syntax_export(char *args)
{
	if (ft_strchr(args, '='))
	{
		if (syntax_with_equal(args))
		{
			glob.exit_status = pr_err("minishell: export: `", args,
					"': not a valid identifier\n", 1);
			return (1);
		}
	}
	else
	{
		if (syntax_without_equal(args))
		{
			glob.exit_status = pr_err("minishell: export: `", args,
					"': not a valid identifier\n", 1);
			return (1);
		}
	}
	return (0);
}
