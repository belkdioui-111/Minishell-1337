/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabrahm <ylabrahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 19:08:06 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/06/12 20:05:11 by ylabrahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	print_error(char *error_msg)
{
	char	*error;

	if (!error_msg)
		return ;
	error = ft_strjoin(ft_strdup("minishell: "), error_msg);
	ft_putstr_fd(error, 2);
	glob.exit_status = 258;
	free(error);
}
