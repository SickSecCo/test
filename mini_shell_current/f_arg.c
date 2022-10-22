/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_arg.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiulian <fgiulian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 18:01:25 by fgiulian          #+#    #+#             */
/*   Updated: 2022/10/21 18:01:25 by fgiulian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_fifth(char *input, int **index)
{
	while (input[(*index)[0] + (*index)[2]] == ' ')
		(*index)[2]++;
	if (input[(*index)[0] + (*index)[2]] == '\0')
		(*index)[0] = (*index)[0] + (*index)[2];
	else
		(*index)[0] = (*index)[0] + (*index)[2] - 1;
}

void	ft_fourth(char *input, int **index)
{
	(*index)[0]++;
	while (input[(*index)[0]] != '>' && input[(*index)[0]] != '\0')
		(*index)[0]++;
}

void	ft_check_if(char *input, t_commands *instruction,
		int **i, char **temp)
{
	if (input[(*i)[0]] == '>')
	{
		ft_first(input, i);
		return ;
	}
	else if (input[(*i)[0]] == '<')
	{
		ft_fourth(input, i);
		return ;
	}
	else if (input[(*i)[0]] == '\'' || input[(*i)[0]] == '\"')
	{
		ft_mix(instruction, input, temp, i);
		return ;
	}
	else if (input[(*i)[0]] == ' ')
	{
		ft_fifth(input, i);
		if (input[(*i)[0] + (*i)[2]] == '<' || input[(*i)[0] + (*i)[2]] == '>')
		{
			(*i)[0] = (*i)[0] + (*i)[2];
			return ;
		}
	}
	ft_cpy(temp, i, input);
}

int	*ft_int_init(int *index)
{
	index[0] = 0;
	index[1] = 0;
	index[4] = 0;
	index[3] = 0;
	return (index);
}

char	*f_arg(char *input, t_commands *instruction)
{
	char	*temp;
	int		*index;

	index = malloc(sizeof(int) * 5);
	index = ft_int_init(index);
	temp = malloc(ft_strlen(input));
	while (input[index[0]] != '\0')
	{
		index[2] = 0;
		ft_check_if(input, instruction, &index, &temp);
	}
	temp[index[1]] = '\0';
	free(index);
	return (temp);
}
