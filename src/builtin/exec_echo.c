/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 22:25:52 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/11 08:53:58 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	set_zeros(int *ret, int *is_n, int *inv_flag)
{
	*ret = 0;
	*is_n = 0;
	*inv_flag = 0;
}

void	print_new_line(int inv_flag, int is_n)
{
	if (inv_flag)
	{
		printf("\n");
		return ;
	}
	if (!is_n)
		printf("\n");
}

void	print_args(t_pre_tokens *args)
{
	while (args)
	{
		printf("%s", args->content);
		args = args->next;
		if (args)
			printf(" ");
	}
}

void	there_is_option(int *is_n, int *inv_flag, t_pre_tokens **args)
{
	int	i;

	i = 0;
	while (*args && (*args)->content[0] == '-')
	{
		i = 0;
		while ((*args)->content[++i])
		{
			if ((*args)->content[i] == 'n')
				*is_n = 1;
			else
				*inv_flag = 1;
		}
		*args = (*args)->next;
	}
}

int	exec_echo(t_command *cmds)
{
	int				ret;
	int				i;
	int				is_n;
	int				inv_flag;
	t_pre_tokens	*args;

	set_zeros(&ret, &is_n, &inv_flag);
	args = cmds->args;
	there_is_option(&is_n, &inv_flag, &args);
	if (inv_flag)
		args = cmds->args;
	print_args(args);
	print_new_line(inv_flag, is_n);
	glob.exit_status = 0;
	return (2);
}
