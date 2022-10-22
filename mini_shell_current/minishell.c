/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiulian <fgiulian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 19:50:31 by fgiulian          #+#    #+#             */
/*   Updated: 2022/10/22 19:15:57 by fgiulian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_env_error;

t_var	*var_initialize(char **envp)
{
	t_var	*var;

	var = malloc(sizeof(t_var));
	var->array_of_inputs = malloc(sizeof(char *) * 100);
	initialize_var(var, envp);
	var->inputs_count = 0;
	return (var);
}

void	free_check_mainloop(char *s, t_bag *bag)
{
	if (bag->err_env_err == 0)
			g_env_error = 0;
	free(s);
	free(bag->mid_bag);
	free(bag);
}

int	free_and_run(t_bag *bag, t_var *var, char *s, char *prompt)
{
	free_no_input(bag, var, s, prompt);
	//  if (strcmp(getenv("SHLVL"), "1"))
	// 	write(1, "\n", 1); da vedere sul computer della 42
	write(1, "\n", 1);
	return (0);
}

void	main_loop_while(t_var *var, struct sigaction sig, char *prompt)
{
	char	*s;
	t_bag	*bag;

	while (1)
	{
		bag = bag_initialize(sig);
		s = readline(prompt);
		if (!s)
			exit(free_and_run(bag, var, s, prompt));
		if (ft_strlen(s) > 0)
		{
			if (main_loop_core(bag, var, s) == 1)
			{
				if (bag->err_env_err == 0)
					g_env_error = 0;
				continue ;
			}
		}
		free_check_mainloop(s, bag);
	}
}

int	main(int argc, char **argv, char **envp)
{
	struct sigaction	mysig;
	char				*prompt;
	t_var				*var;

	(void)argv;
	(void)argc;
	sigemptyset(&mysig.sa_mask);
	mysig.sa_flags = 0;
	mysig.sa_sigaction = signal_handler;
	set_termios(1);
	prompt = ft_strdup("\033[93m\033[1mminishell\033[34m$\033[0m ");
	var = var_initialize(envp);
	main_loop_while(var, mysig, prompt);
}
