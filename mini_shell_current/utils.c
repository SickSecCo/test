/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiulian <fgiulian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 19:50:54 by fgiulian          #+#    #+#             */
/*   Updated: 2022/10/20 20:50:57 by fgiulian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] != '\0' && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	strsplitwrite_(char **str2, char *str, char del, int starter)
{
	int			i;
	int			j;
	static int	x;

	i = 0;
	while (str[i] != '\0')
	{
		if (charsep(str[i], del) == 1)
			i++;
		else
		{
			x++;
			j = 0;
			while (charsep(str[i + j], del) == 0)
				j++;
			str2[starter] = (char *)malloc(sizeof(char) * (j + 1));
			write_oneword(str2[starter], str + i, del);
			i = i + j;
			starter++;
		}
	}
	return (starter - 1);
}

char	**split_basic(char *s, char c, int *starter)
{
	int		chars;
	int		count;
	char	**array;

	count = *starter;
	if (s == NULL)
		return (NULL);
	chars = count_chars(s, c);
	array = malloc(sizeof(char *) * (chars));
	count = strsplitwrite_(array, s, c, count);
	*starter = count;
	return (array);
}

void	check_command(t_bag *bag, t_var *var)
{
	if (!ft_strcmp(bag->mid_bag->instructions->command, "echo"))
		ft_echo(bag->mid_bag->instructions, var);
	else if (!ft_strcmp(bag->mid_bag->instructions->command, "cd"))
		ft_cd(bag, var);
	else if (!ft_strcmp(bag->mid_bag->instructions->command, "pwd"))
		ft_pwd();
	else if (!ft_strcmp(bag->mid_bag->instructions->command, "export"))
		ft_export(var, bag);
	else if (!ft_strcmp(bag->mid_bag->instructions->command, "unset"))
		ft_unset(var, bag);
	else if (!ft_strcmp(bag->mid_bag->instructions->command, "env"))
		ft_env(var);
	else if (!ft_strcmp(bag->mid_bag->instructions->command, "exit"))
	{
		free(var);
		exit(0);
	}
	return ;
}
