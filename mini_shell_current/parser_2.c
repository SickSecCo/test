/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiulian <fgiulian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 17:02:17 by fgiulian          #+#    #+#             */
/*   Updated: 2022/10/22 17:02:17 by fgiulian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_pipe_2(char *input, t_bag *bag, size_t *i, int *j)
{
	bag->mid_bag->raw_command = ft_substr(input, (*j), (*i) - (*j));
	bag->mid_bag->next = malloc(sizeof(t_tree));
	bag->mid_bag->next->prev = bag->mid_bag;
	bag->mid_bag = bag->mid_bag->next;
	(*j) = (*i) + 1;
	(*i)++;
	bag->commands_count++;
}

int	ft_echo_option(t_tree *bag, int i)
{
	while (bag->raw_command[i] == ' ')
		i++;
	if (bag->raw_command[i] == '-' && bag->raw_command[i + 1] == 'n')
	{
		bag->instructions->echo_option = 1;
		i += 2;
	}
	return (i);
}

void	ft_arg_split(t_tree *bag, int *i, int *j)
{
	while (bag->raw_command[(*i)] == ' ')
		(*i)++;
	(*j) = (*i);
	while (bag->raw_command[(*i)] != ' ' && bag->raw_command[(*i)] != '\0')
		(*i)++;
	bag->instructions->command = ft_substr(bag->raw_command, (*j), (*i) - (*j));
	if (!ft_strcmp(bag->instructions->command, "echo"))
		(*i) = ft_echo_option(bag, (*i));
	while (bag->raw_command[(*i)] == ' ')
		(*i)++;
	(*j) = (*i);
	while (bag->raw_command[(*i)] != '\0')
		(*i)++;
	bag->instructions->argument
		= ft_substr(bag->raw_command, (*j), (*i) - (*j));
}

t_index	*ft_index(void)
{
	t_index	*index;

	index = malloc(sizeof(t_index));
	index->count = 0;
	index->i = 0;
	index->j = 0;
	index->type = -1;
	return (index);
}

void	ft_mult_quotes(t_index *index, t_commands *instruction)
{
	index->type = 1;
	index->count++;
	if (index->count == 1)
	{
		instruction->quote_flag[index->j] = 2;
		instruction->quote_start[index->j] = index->i;
	}
	if (index->count == 2)
	{
		instruction->quote_end[index->j] = index->i;
		index->j++;
		index->count = 0;
		index->type = -1;
		instruction->quote_count++;
	}
}
