/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiulian <fgiulian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 19:50:24 by fgiulian          #+#    #+#             */
/*   Updated: 2022/10/19 19:50:24 by fgiulian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_while(t_bag *bag)
{
	free(bag->mid_bag->instructions->output_arg);
	free(bag->mid_bag->instructions->command);
	free(bag->mid_bag->instructions->argument);
	free(bag->mid_bag->instructions->quote_flag);
	free(bag->mid_bag->instructions->quote_start);
	free(bag->mid_bag->instructions->quote_end);
	free(bag->mid_bag->instructions->arr_file);
	free(bag->mid_bag->instructions->arr_delimiters);
	if (bag->mid_bag->instructions->arr_execve_index > 0)
	{
		while (bag->mid_bag->instructions->arr_execve_index > 0)
		{
			free(bag->mid_bag->instructions->arr_execve
			[bag->mid_bag->instructions->arr_execve_index - 1]);
			bag->mid_bag->instructions->arr_execve_index--;
		}
	}
	free(bag->mid_bag->instructions->arr_execve);
	free(bag->mid_bag->instructions);
}

void	loop_free(t_var *var, t_bag *bag, int commands_count2)
{
	int	i;

	while (commands_count2 > 0)
	{
		if (bag->mid_bag->instructions->file_input)
			free(bag->mid_bag->instructions->file_input);
		if (bag->mid_bag->instructions->file_output)
			free(bag->mid_bag->instructions->file_output);
		free_while(bag);
		if (--commands_count2 > 0)
		{
			bag->mid_bag = bag->mid_bag->prev;
			free(bag->mid_bag->next);
		}
	}
	i = 0;
	while (i < var->count + 1)
	{
		free(var->var_execve[i]);
		var->var_execve[i] = NULL;
		i++;
	}
	free(var->var_execve);
	var->var_execve = NULL;
	unlink("h.txt");
}

void	free_no_input_2(t_bag *bag, t_var *var, int i)
{
	while (i < var->count)
	{
		free(var->var_name[i]);
		free(var->var_value[i]);
		i++;
	}
	free(var->name);
	free(var->value);
	free(bag->mid_bag);
	free(bag);
	free(var->var_value);
	free(var->var_name);
	if (var->var_execve)
	{
		i = 0;
		while (i < var->count + 1)
			free(var->var_execve[i++]);
		free(var->var_execve);
		var->var_execve = NULL;
	}
}

void	free_no_input(t_bag *bag, t_var *var, char *s, char *prompt)
{
	int	i;

	i = 0;
	free(s);
	free_no_input_2(bag, var, i);
	while (i < var->inputs_count)
		free(var->array_of_inputs[i++]);
	free(var->array_of_inputs);
	var->array_of_inputs = NULL;
	free(prompt);
	free(var);
	rl_clear_history();
	set_termios(0);
}
