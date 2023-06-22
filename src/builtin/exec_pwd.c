/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 22:28:13 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/22 12:41:13 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	exec_pwd(void)
{
	ft_putstr_fd(search_in_env_and_return_value(g_glob.env, "PWD"), 1);
	ft_putstr_fd("\n", 1);
	g_glob.exit_status = 0;
	return (3);
}
