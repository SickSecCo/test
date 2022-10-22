/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiulian <fgiulian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 19:50:41 by fgiulian          #+#    #+#             */
/*   Updated: 2022/10/22 18:29:49 by fgiulian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse(t_bag *bag, char *input, t_var *var)
{
	int	commands_count;
	int	i;
	int	k;

	i = 0;
	bag->commands_count = parse_string_pipe(bag, input);
	commands_count = bag->commands_count;
	var->var_execve = malloc(sizeof(char *) * var->count + 1);
	if (commands_count == -1)
		return (commands_count);
	while (++i < commands_count)
		bag->mid_bag = bag->mid_bag->prev;
	if (commands_count > 1)
		bag->mid_bag->pipe_yes_not = 1;
	k = parse_string_commands(bag->mid_bag, commands_count, var);
	if (k == -1)
		return (k);
	return (0);
}

int	parse_string_pipe(t_bag *bag, char *input)
{
	size_t	i;
	int		j;

	i = 0;
	j = 0;
	bag->commands_count = 0;
	while (i <= ft_strlen(input))
	{
		if ((input[i] != '|' && input[i] != '\0'))
			i++;
		if (input[i] == '|' && input[i + 1] == '|')
		{
			printf("syntax error near unexpected token '|'\n");
			return (-1);
		}
		else if (input[i] == '|')
			parse_pipe_2(input, bag, &i, &j);
		else if (i == ft_strlen(input))
		{
			bag->mid_bag->raw_command = ft_substr(input, j, i - j);
			i++;
			bag->commands_count++;
		}
	}
	return (bag->commands_count);
}

int	parse_string_commands(t_tree *bag, int commands_count, t_var *var)
{
	int	i;
	int	j;
	int	k;

	while (commands_count > 0)
	{
		i = 0;
		bag = initialize_instructions(bag);
		ft_arg_split(bag, &i, &j);
		k = check_quotes(bag->instructions);
		if (!ft_strcmp(bag->instructions->command, "unset"))
			check_arguments_unset(var, bag);
		ft_check_redi(bag->instructions);
		free(bag->raw_command);
		bag->raw_command = NULL;
		if (commands_count - 1 > 0)
			bag = bag->next;
		commands_count--;
	}
	if (k != -1)
		return (0);
	else
		return (k);
}

void	ft_sing_quote(t_index *index, t_commands *instruction)
{
	index->type = 0;
	index->count++;
	if (index->count == 1)
	{
		instruction->quote_flag[index->j] = 1;
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

int	check_quotes(t_commands *instruction)
{
	t_index	*index;

	index = ft_index();
	while (instruction->argument[index->i])
	{
		if (instruction->argument[index->i] == '\'' && index->type != 1)
			ft_sing_quote(index, instruction);
		else if (instruction->argument[index->i] == '\"' && index->type != 0)
			ft_mult_quotes(index, instruction);
		index->i++;
	}
	if (index->count == 1)
	{
		printf("ERROR: MISSING QUOTE\n");
		free(index);
		return (-1);
	}
	free(index);
	return (0);
}
