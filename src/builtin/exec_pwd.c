/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 22:28:13 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/15 14:54:27 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	exec_pwd(void)
{
	ft_putstr_fd(search_in_env(glob.env, "PWD"), 1);
	ft_putstr_fd("\n", 1);
	glob.exit_status = 0;
	return (3);
}
