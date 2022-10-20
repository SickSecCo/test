/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiulian <fgiulian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 19:50:31 by fgiulian          #+#    #+#             */
/*   Updated: 2022/10/20 20:41:02 by fgiulian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int env_error;

void	main_loop_fork_multiple_commands(t_bag *bag, t_var *var, int commands_count)
{
	struct sigaction	sig_struct;

	sigemptyset(&sig_struct.sa_mask);
	sig_struct.sa_flags = 0;
	sig_struct.sa_sigaction = signal_handler_1;
	sigaction(SIGINT, &sig_struct, NULL);
	sigaction(SIGQUIT, &sig_struct, NULL);


	t_fork *f_stats;
	int	fd[2];
	int	fd_2[2];
	int	fd_backup[2];

	int	pid, count, pid_1, pid_1_ex_status;

	fd_backup[0] = dup(STDOUT_FILENO);
	fd_backup[1] = dup(STDIN_FILENO);
	f_stats = malloc(sizeof(t_fork));
	f_stats->count = 1;
	bag->mid_bag->instructions->builtin_yes_not = 0;
	pipe(fd);
	is_built_in_set(bag, 1);
	f_stats->pid = fork();
	if (f_stats->pid == 0)
		exit(fork_loop_first_child(bag, fd, var));
	else
	{
		close(fd[1]);
		waitpid(f_stats->pid, NULL, 0);
		if (bag->mid_bag->instructions->out_redirect_type && !bag->mid_bag->instructions->builtin_yes_not)
		{
			read_util(bag, fd);
			close(fd[0]);
			bag->mid_bag->instructions->echo_option = 1;
			ft_echo(bag->mid_bag->instructions, var);
		}
	}
	while (f_stats->count < commands_count)
	{
		pipe(fd_2);
		bag->mid_bag = bag->mid_bag->next;
		bag->mid_bag->instructions->builtin_yes_not = 0;
		is_built_in_set(bag, 1);
		f_stats->pid_1 = fork();
		if (f_stats->pid_1 == 0)
		{
			if (bag->mid_bag->instructions->in_redirect_type || bag->mid_bag->instructions->heredocs_switch)
				in_redirect_fork_util(bag->mid_bag->instructions);
			else
				dup2(fd[0], STDIN_FILENO);
			if (commands_count - f_stats->count > 1 || bag->mid_bag->instructions->out_redirect_type)
				dup2(fd_2[1], STDOUT_FILENO);
			int a = (fork_loop_second_child(bag, fd, fd_2, var));
			exit(a);
		}
		else
		{
			close(fd_2[1]);
			waitpid(f_stats->pid_1, &f_stats->pid_1_ex_status, 0);
			if (!bag->mid_bag->instructions->out_redirect_type)
				dup2(fd_2[0], fd[0]);
			close(fd_2[0]);
			if (bag->mid_bag->instructions->out_redirect_type && !bag->mid_bag->instructions->builtin_yes_not)
			{
				read_util(bag, fd);
				bag->mid_bag->instructions->echo_option = 1;
				ft_echo(bag->mid_bag->instructions, var);
			}
			f_stats->count++;
		}
	}
	close(fd[0]);
	dup2(fd_backup[1], STDIN_FILENO);
	dup2(fd_backup[0], STDOUT_FILENO);
	close(fd_backup[0]);
	close(fd_backup[1]);
	if (env_error != 130 && env_error != 131)
		env_error = WEXITSTATUS(f_stats->pid_1_ex_status);
	bag->err_env_err = 1;
	free(f_stats);
}

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
	{
		check_command(bag, var);
		return ;
	}
	ft_fork(bag, var);
	if (bag->mid_bag->instructions->in_redirect_type || bag->mid_bag->instructions->out_redirect_type)
	{
		bag->mid_bag->instructions->echo_option = 1;
		ft_echo(bag->mid_bag->instructions, var);
	}
}

int	main_loop_core(t_bag *bag, t_var *var, char *s)
{
	int commands_count2;
	int	check;

	if (check_inputs(var, s))
	{
		var->inputs_count++;
		add_history(s);
	}
	check = parse(bag, s, var);
	ft_var(var);
	commands_count2 = bag->commands_count;
	if (check == -1)
	{
		loop_free(var, bag, commands_count2);
		return (1);
	}
	else if (!ft_strcmp(bag->mid_bag->instructions->command, "exit"))
	{
		loop_free(var, bag, commands_count2);
		return(-1);
	}
	if (env_error == 130 && bag->mid_bag->instructions->in_redirect_type == 2)
	{
		bag->err_env_err = 1;
		return (0);
	}
	if (bag->mid_bag->pipe_yes_not)
		main_loop_fork_multiple_commands(bag, var, bag->commands_count);
	else
		main_loop_fork(bag, var);
	loop_free(var, bag, commands_count2);
	return (0);
}

void	main_loop(struct sigaction sig, char **envp)
{
	char *prompt;
	char *s;
	t_var	*var;
	t_bag	*bag;
	int	code;

	prompt = ft_strdup("\033[93m\033[1mminishell\033[34m$\033[0m ");
	var = malloc(sizeof(t_var));
	var->array_of_inputs = malloc(sizeof(char *) * 100);
	initialize_var(var, envp);
	var->inputs_count = 0;
	code = 0;
	while (1)
	{
		bag = malloc(sizeof(t_bag));
		bag->mid_bag = malloc(sizeof(t_tree));
		bag->err_env_err = 0;
		bag->mid_bag->pipe_yes_not = 0;
		sigaction(SIGINT, &sig, NULL);
		signal(SIGQUIT, SIG_IGN);
		s = readline(prompt);
		if (!s)
		{
			free_no_input(bag, var, s, prompt);
			/* if (strcmp(getenv("SHLVL"), "1"))
			 	write(1, "\n", 1); da vedere sul computer della 42 */
			write(1, "\n", 1); 
			exit(0);
		}
		if (ft_strlen(s) > 0)
		{
			code = main_loop_core(bag, var, s);
			if (code == -1)
				exit(0);
			else if (code == 1)
			{
				if (bag->err_env_err == 0)
					env_error = 0;
				continue;
			}
		}
		if (bag->err_env_err == 0)
			env_error = 0;
		free(s);
		free(bag->mid_bag);
		free(bag);
	}
}

int main(int argc, char **argv, char **envp)
{
	struct sigaction	mysig;

	sigemptyset(&mysig.sa_mask);
	mysig.sa_flags = 0;
	mysig.sa_sigaction = signal_handler;
	(void)argv;
	(void)argc;
	set_termios(1);

	main_loop(mysig, envp);
}
