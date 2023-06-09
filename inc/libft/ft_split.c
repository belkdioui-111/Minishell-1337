/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 16:25:15 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/06/09 13:50:00 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_counter(char *s, char c)
{
	int	i;
	int	cnt;

	i = 0;
	cnt = 0;
	while (s[i] != '\0')
	{
		if ((s[i] != '\0') && (s[i] != c))
		{
			cnt++;
			while ((s[i] != '\0') && (s[i] != c))
				i++;
		}
		while ((s[i] == c) && (s[i]))
			i++;
	}
	return (cnt);
}

static char	**ft_allocater(char *s, char c, char **strs)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	k = 0;
	while (s[i] != '\0')
	{
		j = 0;
		if ((s[i] != '\0') && (s[i] != c))
		{
			while ((s[i] != '\0') && (s[i] != c))
			{
				j++;
				i++;
			}
			strs[k] = (char *) ft_calloc(j + 1, sizeof(char));
			if (!strs[k])
				return (NULL);
			k++;
		}
		while ((s[i] == c) && (s[i]))
			i++;
	}
	return (strs);
}

static char	**ft_filler(char *s, char c, char **strs)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	k = 0;
	while (s[i] != '\0')
	{
		if ((s[i] != '\0') && (s[i] != c))
		{
			j = 0;
			while ((s[i] != '\0') && (s[i] != c))
			{
				strs[k][j] = s[i];
				i++;
				j++;
			}
			k++;
		}
		while ((s[i] == c) && (s[i]))
			i++;
	}
	return (strs);
}

char	**ft_split(char *s, char c)
{
	int		count;
	char	**arrays;

	if (!s)
		return (NULL);
	count = ft_counter(s, c);
	arrays = (char **) ft_calloc(count + 1, sizeof(char *));
	if (!arrays)
		return (NULL);
	if (!count)
		return (arrays);
	return (ft_filler(s, c, (ft_allocater(s, c, arrays))));
}
