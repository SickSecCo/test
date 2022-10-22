/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiulian <fgiulian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 19:50:38 by fgiulian          #+#    #+#             */
/*   Updated: 2022/10/22 16:54:31 by fgiulian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	figure_count(void)
{
	int	k;
	int	j;

	k = g_env_error;
	j = 0;
	while (k / 10 > 0)
	{
		j++;
		k = k / 10;
	}
	return (j);
}

void	ft_putnbr(char **str, int j, int k, int i)
{
	if (k >= 10)
	{
		ft_putnbr(str, j, k / 10, i - 1);
		(*str)[j + i] = k % 10 + 48;
	}
	else
		(*str)[j + i] = k + 48;
}

int	check_inputs(t_var *var, char *input)
{
	static int	i;

	if (var->inputs_count == 0)
		var->array_of_inputs[var->inputs_count]
			= ft_substr(input, 0, ft_strlen(input));
	else
	{
		if (!ft_strcmp(input, var->array_of_inputs[i]))
			return (0);
		var->array_of_inputs[var->inputs_count]
			= ft_substr(input, 0, ft_strlen(input));
		i++;
	}
	return (1);
}

int	ft_strcmp_mini(char *str1, char *str2)
{
	int	i;

	i = 0;
	while (str1[i] != '\0')
	{
		if (!ft_strcmp(&str1[i], str2))
			return (0);
		i++;
	}
	return (1);
}
