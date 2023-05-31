/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabrahm <ylabrahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 15:36:58 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/05/22 03:46:33 by ylabrahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char *s, unsigned int start, size_t len)
{
	char	*str_rete;
	size_t	i;

	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (ft_calloc(1, sizeof(char)));
	i = 0;
	str_rete = (char *) ft_calloc((len + 1), sizeof(char));
	if (!str_rete)
		return (NULL);
	while (i < len)
	{
		str_rete[i] = s[start + i];
		i++;
	}
	// free(s);
	return (str_rete);
}
