/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabrahm <ylabrahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 17:16:38 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/05/28 19:13:59 by ylabrahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_cat(char *ret_str, char *s1, char *s2)
{
	size_t	len_1;
	size_t	len_2;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	len_1 = ft_strlen(s1);
	len_2 = ft_strlen(s2);
	while (i < len_1)
	{
		ret_str[i] = s1[i];
		i++;
	}
	while (j < len_2)
	{
		ret_str[i] = s2[j];
		j++;
		i++;
	}
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*ret_str;
	int		s1_len;
	int		s2_len;

	if ((s1 == 0) || (s2 == 0))
		return (0);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	ret_str = (char *) ft_calloc((s1_len + s2_len + 1), sizeof(char));
	if (!ret_str)
		return (0);
	ft_cat(ret_str, s1, s2);
	free(s1);
	return (ret_str);
}
