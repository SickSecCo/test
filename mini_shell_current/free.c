#include "minishell.h"

void    loop_free(t_var *var, t_bag *bag, int commands_count2)
{
	if (var->name != NULL)
		memset(var->name, 0, ft_strlen(var->name));
	if (var->value != NULL)
		memset(var->value, 0, ft_strlen(var->value));
	while(commands_count2 > 0)
	{
		if (bag->mid_bag->instructions->file_input)
		{
			free(bag->mid_bag->instructions->file_input);
			bag->mid_bag->instructions->file_input = NULL;
		}
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
				free(bag->mid_bag->instructions->arr_execve[bag->mid_bag->instructions->arr_execve_index - 1]);
				bag->mid_bag->instructions->arr_execve_index--;
			}
		}
		free(bag->mid_bag->instructions->arr_execve);
		free(bag->mid_bag->instructions);
		bag->mid_bag->instructions = NULL;
		commands_count2--;
		if (commands_count2 > 0)
		{
			bag->mid_bag = bag->mid_bag->prev;
			free(bag->mid_bag->next);
		}
	}
	int i = 0;
	while (i < var->count)
	{
		free(var->var_execve[i]);
		var->var_execve[i] = NULL;
		i++;
	}
	free(var->var_execve);
	var->var_execve = NULL;
}

void	free_no_input(t_bag *bag, t_var *var, char *s, char *prompt)
{
	free(s);
	int i = 0;
	while (i < var->count)
	{
		free(var->var_name[i]);
		free(var->var_value[i]);
		i++;
	}
	free(var->name);	// forse
	free(var->value);	// forse
	free(bag->mid_bag);
	free(bag);
	free(var->var_value);
	free(var->var_name);
	if (var->var_execve)
	{
		i = 0;
		while (i < var->count)
			free(var->var_execve[i++]);
		free(var->var_execve);
		var->var_execve = NULL;
	}
	i = 0;
	while (i < var->inputs_count)
		free(var->array_of_inputs[i++]);
	free(var->array_of_inputs);	//questo se scommentato fa balzare i leak a 80k no idea why
	var->array_of_inputs = NULL;
	free(prompt);
	free(var);	//questo mi sa di si
	rl_clear_history();
	set_termios(0);
}
