/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiulian <fgiulian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 16:17:04 by fgiulian          #+#    #+#             */
/*   Updated: 2022/10/21 16:17:04 by fgiulian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_go(char *str, int *index)
{
	index[0]++;
	while ((str[index[0]] >= 48 && str[index[0]] <= 57)
		|| (str[index[0]] >= 65 && str[index[0]] <= 90)
		|| (str[index[0]] >= 97 && str[index[0]] <= 122))
		index[0]++;
}

void	ft_no_name(char *dest, char *str, int *index)
{
	if (str[index[0]] == '$' && str[index[0] + 1] == '\0')
	{
		dest[index[1]] = str[index[0]];
		index[1]++;
	}
	index[0]++;
	while ((str[index[0]] >= 48 && str[index[0]] <= 57)
		|| (str[index[0]] >= 65 && str[index[0]] <= 90)
		|| (str[index[0]] >= 97 && str[index[0]] <= 122))
		index[0]++;
	if (str[index[0]] == '\0')
		return ;
	else
		index[3]++;
}

void	ft_int_handle(char *dest, int *index)
{
	index[1] += figure_count();
	ft_putnbr(&dest, index[1], g_env_error, 0);
	index[0] += 2;
	index[1]++;
}

void	ft_advance(t_handler **handle, char *str, t_commands *instruction)
{
	(*handle)->dest[(*handle)->index[1]] = str[(*handle)->index[0]];
	(*handle)->index[1]++;
	(*handle)->index[0]++;
	if ((*handle)->index[0] == instruction->quote_end[(*handle)->index[2]] + 1)
		(*handle)->index[2]++;
}

void	init_handle(t_handler **handle, t_commands *instruction)
{
	(*handle)->dest = malloc(sizeof(char)
			* ft_strlen(instruction->output_arg) + 150);
	(*handle)->index = malloc(sizeof(int) * 5);
	(*handle)->index[0] = 0;
	(*handle)->index[1] = 0;
	(*handle)->index[2] = 0;
	(*handle)->index[3] = 0;
	(*handle)->index[4] = 0;
}
