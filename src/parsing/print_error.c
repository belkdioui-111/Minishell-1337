/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabrahm <ylabrahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 19:08:06 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/05/28 19:23:00 by ylabrahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	print_error(char *error_msg)
{
	char	*error;

	if (!error_msg)
		return ;
	error = ft_strjoin(ft_strdup("monoshell-1.0: "), error_msg);
	error = ft_strjoin(error, "\n");
	error = ft_colorize(error, "red");
	ft_putstr_fd(error, 2);
	free(error);
}
