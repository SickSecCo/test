/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_arg_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiulian <fgiulian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 18:19:16 by fgiulian          #+#    #+#             */
/*   Updated: 2022/10/21 18:19:16 by fgiulian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_first(char *input, int **index)
{
	(*index)[0]++;
	if (input[(*index)[0]] == '>')
		(*index)[0]++;
	while (input[(*index)[0]] == ' ')
		(*index)[0]++;
	while (input[(*index)[0]] != ' ' && input[(*index)[0]] != '\0')
		(*index)[0]++;
}

void	ft_second(t_commands *instruction, char *input,
		char **temp, int **index)
{
	instruction->quote_start[(*index)[3]] = (*index)[0] - (*index)[4];
	(*index)[0]++;
	if (input[(*index)[0]] == '\'')
	{
		instruction->quote_end[(*index)[3]] = (*index)[0];
		(*index)[0]++;
		return ;
	}
	(*index)[4]++;
	while (input[(*index)[0]] != '\'' && input[(*index)[0]] != '\0')
	{
		(*temp)[(*index)[1]] = input[(*index)[0]];
		(*index)[1]++;
		(*index)[0]++;
	}
	instruction->quote_end[(*index)[3]] = (*index)[0] - (*index)[4] - 1;
	if (input[(*index)[0]] == '\'')
		(*index)[0]++;
	(*index)[4]++;
	(*index)[3]++;
}

void	ft_third(t_commands *instruction, char *input, char **temp, int **index)
{
	instruction->quote_start[(*index)[3]] = (*index)[0] - (*index)[4];
	(*index)[0]++;
	if (input[(*index)[0]] == '\"')
	{
		instruction->quote_end[(*index)[3]] = (*index)[0];
		(*index)[0]++;
		return ;
	}
	(*index)[4]++;
	while (input[(*index)[0]] != '\"' && input[(*index)[0]] != '\0')
	{
		(*temp)[(*index)[1]] = input[(*index)[0]];
		printf("%c---%d\n", input[(*index)[0]], (*index)[0]);
		(*index)[1]++;
		(*index)[0]++;
	}
	instruction->quote_end[(*index)[3]] = (*index)[0] - (*index)[4] - 1;
	if (input[(*index)[0]] == '\"')
		(*index)[0]++;
	(*index)[4]++;
	(*index)[3]++;
}

void	ft_cpy(char **temp, int **index, char *input)
{
	(*temp)[(*index)[1]] = input[(*index)[0]];
	(*index)[1]++;
	(*index)[0]++;
}

void	ft_mix(t_commands *instruction, char *input, char **temp, int **index)
{
	if (input[(*index)[0]] == '\'')
		ft_second(instruction, input, temp, index);
	else if (input[(*index)[0]] == '\"')
		ft_third(instruction, input, temp, index);
}
