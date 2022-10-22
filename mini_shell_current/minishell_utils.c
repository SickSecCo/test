/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiulian <fgiulian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 18:30:43 by fgiulian          #+#    #+#             */
/*   Updated: 2022/10/22 18:30:43 by fgiulian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	main_loop_fork(t_bag *bag, t_var *var)
{
	struct sigaction	sig_struct;

	sigemptyset(&sig_struct.sa_mask);
	sig_struct.sa_flags = 0;
	sig_struct.sa_sigaction = signal_handler_1;
	sigaction(SIGINT, &sig_struct, NULL);
	sigaction(SIGQUIT, &sig_struct, NULL);
	if (!ft_strcmp(bag->mid_bag->instructions->command, "export")
		|| !ft_strcmp(bag->mid_bag->instructions->command, "echo")
		|| !ft_strcmp(bag->mid_bag->instructions->command, "unset")
		|| !ft_strcmp(bag->mid_bag->instructions->command, "cd"))
		return (check_command(bag, var));
	if (ft_fork(bag, var))
		bag->err_env_err = 1;
	if (bag->mid_bag->instructions->in_redirect_type
		|| bag->mid_bag->instructions->out_redirect_type)
	{
		bag->mid_bag->instructions->echo_option = 1;
		ft_echo(bag->mid_bag->instructions, var);
	}
}

int	main_loop_core_utils(t_var *var, t_bag *bag, char *s)
{
	int	check;

	if (check_inputs(var, s))
	{
		var->inputs_count++;
		add_history(s);
	}
	check = parse(bag, s, var);
	return (check);
}

int	main_loop_core_utils_1(t_var *var, t_bag *bag,
	int commands_count2, int check)
{
	if (check == -1)
	{
		loop_free(var, bag, commands_count2);
		return (1);
	}
	else if (!ft_strcmp(bag->mid_bag->instructions->command, "exit"))
	{
		loop_free(var, bag, commands_count2);
		exit(0);
	}
	return (0);
}

int	main_loop_core(t_bag *bag, t_var *var, char *s)
{
	int	commands_count2;
	int	check;

	check = main_loop_core_utils(var, bag, s);
	commands_count2 = bag->commands_count;
	ft_var(var);
	if (main_loop_core_utils_1(var, bag, commands_count2, check))
		return (1);
	if (g_env_error == 130 && bag->mid_bag->instructions->in_redirect_type == 2)
	{
		bag->err_env_err = 1;
		return (0);
	}
	if (bag->mid_bag->pipe_yes_not)
		fork_multiple_commands(bag, var, bag->commands_count);
	else
		main_loop_fork(bag, var);
	loop_free(var, bag, commands_count2);
	return (0);
}

t_bag	*bag_initialize(struct sigaction sig)
{
	t_bag	*bag;

	bag = malloc(sizeof(t_bag));
	bag->mid_bag = malloc(sizeof(t_tree));
	bag->err_env_err = 0;
	bag->mid_bag->pipe_yes_not = 0;
	sigaction(SIGINT, &sig, NULL);
	signal(SIGQUIT, SIG_IGN);
	return (bag);
}
