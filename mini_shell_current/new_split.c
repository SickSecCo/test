/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiulian <fgiulian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 19:50:35 by fgiulian          #+#    #+#             */
/*   Updated: 2022/10/20 20:21:31 by fgiulian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	charsep(char c, char del)
{
	if (c == del || c == '\0')
		return (1);
	return (0);
}

int	count_chars(char *str, char del)
{
	int	x;
	int	chars;

	x = 0;
	chars = 0;
	while (str[x] != '\0')
	{
		if (charsep(str[x + 1], del) == 1 && charsep(str[x], del) == 0)
			chars++;
		x++;
	}
	return (chars);
}

void	write_oneword(char *dst, char *src, char del)
{
	int	i;

	i = 0;
	while (charsep(src[i], del) == 0)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
}

int	strsplitwrite(char **str2, char *str, char del, int starter)
{
	int	i;
	int	j;

	i = 0;
	while (str[i] != '\0')
	{
		if (charsep(str[i], del) == 1)
			i++;
		else
		{
			j = 0;
			while (charsep(str[i + j], del) == 0)
				j++;
			str2[starter] = (char *)malloc(sizeof(char) * (j + 1));
			write_oneword(str2[starter], str + i, del);
			i = i + j;
			starter++;
		}
	}
	return (starter);
}

char	**ft_splitto(char **array, char *s, char c, int *starter)
{
	int		count;

	count = *starter;
	if (s == NULL)
		return (NULL);
	count = strsplitwrite(array, s, c, count);
	*starter = count;
	return (array);
}
