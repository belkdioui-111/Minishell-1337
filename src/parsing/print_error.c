/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 19:08:06 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/06/22 12:41:13 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	print_error(char *error_msg, int error_num)
{
	if (!error_msg)
		return ;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(error_msg, 2);
	g_glob.exit_status = error_num;
}
