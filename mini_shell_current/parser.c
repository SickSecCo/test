/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiulian <fgiulian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 19:50:41 by fgiulian          #+#    #+#             */
/*   Updated: 2022/10/19 20:56:30 by fgiulian         ###   ########.fr       */
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
	while (++i < commands_count)
		bag->mid_bag = bag->mid_bag->prev;
	if (commands_count > 1)
		bag->mid_bag->pipe_yes_not = 1;
	k = parse_string_commands(bag->mid_bag, commands_count, var);
	var->var_execve = malloc(sizeof(char *) * var->count + 1);
	if (k == -1)
		return k;
	else
		return (0);
}

int	parse_string_pipe(t_bag *bag, char *input)
{
	size_t i;
	int	j;
	int commands_count;

	i = 0;
	j = 0;
	commands_count = 0;
	while (i <= ft_strlen(input))
	{
		if ((input[i] != '|' && input[i] != '\0') || (input[i] ==  '|' && input[i + 1] == '|'))
		{
			i++;
			if (input[i] ==  '|' && input[i + 1] == '|')
			{
				printf("syntax error near unexpected token '|'\n");
				return(-1);
			}
		}
		else if (input[i] == '|')
		{
			bag->mid_bag->raw_command = ft_substr(input, j, i - j);
			bag->mid_bag->next = malloc(sizeof(t_tree));
			bag->mid_bag->next->prev = bag->mid_bag;
			bag->mid_bag = bag->mid_bag->next;
			j = i + 1;
			i++;
			commands_count++;
		}
		else if (i == ft_strlen(input))
		{
			bag->mid_bag->raw_command = ft_substr(input, j, i - j);
			i++;
			commands_count++;
		}
	}
	return (commands_count);
}

int	parse_string_commands(t_tree *bag, int commands_count, t_var *var)
{
	int i;
	int	j;
	int	k;

	while (commands_count > 0)
	{
		i = 0;
		bag = initialize_instructions(bag);
		while (bag->raw_command[i] == ' ')
			i++;
		j = i;
		while (bag->raw_command[i] != ' ' && bag->raw_command[i] != '\0')
			i++;
		bag->instructions->command = ft_substr(bag->raw_command, j, i - j);
		if (!ft_strcmp(bag->instructions->command, "echo"))
		{
			while (bag->raw_command[i] == ' ')
				i++;
			if (bag->raw_command[i] == '-' && bag->raw_command[i + 1] == 'n')
			{
				bag->instructions->echo_option = 1;
				i += 2;
			}
		}
		while (bag->raw_command[i] == ' ')
			i++;
		j = i;
		while (bag->raw_command[i] != '\0')
			i++;
		bag->instructions->argument = ft_substr(bag->raw_command, j, i - j);
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
		return k;
}

int	check_quotes(t_commands *instruction)
{
	int		i;
	int		j;
	int		count;
	int		type;

	i = 0;
	j = 0;
	count = 0;
	type = -1;
	while (instruction->argument[i])
	{
		if (instruction->argument[i] == '\'' && type != 1)
		{
			type = 0;
			count++;
			if (count == 1)
			{
				instruction->quote_flag[j] = 1;
				instruction->quote_start[j] = i;
			}
			if (count == 2)
			{
				instruction->quote_end[j] = i;
				j++;
				count = 0;
				type = -1;
				instruction->quote_count++;
			}
		}
		else if (instruction->argument[i] == '\"' && type != 0)
		{
			type = 1;
			count++;
			if (count == 1)
			{
				instruction->quote_flag[j] = 2;
				instruction->quote_start[j] = i;
			}
			if (count == 2)
			{
				instruction->quote_end[j] = i;
				j++;
				count = 0;
				type = -1;
				instruction->quote_count++;
			}
		}
		i++;
	}
	if (count == 1)
	{
		printf("ERROR: MISSING QUOTE\n");
		return (-1);
	}
	return 0;
}
