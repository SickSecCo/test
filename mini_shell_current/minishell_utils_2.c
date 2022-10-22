/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiulian <fgiulian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 18:31:37 by fgiulian          #+#    #+#             */
/*   Updated: 2022/10/22 18:31:37 by fgiulian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_fork	*set_signal_fork_multi(t_bag *bag, int command_count)
{
	t_fork				*f_stats;
	struct sigaction	sig_struct;

	sigemptyset(&sig_struct.sa_mask);
	sig_struct.sa_flags = 0;
	sig_struct.sa_sigaction = signal_handler_1;
	sigaction(SIGINT, &sig_struct, NULL);
	sigaction(SIGQUIT, &sig_struct, NULL);
	f_stats = malloc(sizeof(t_fork));
	f_stats->count = 1;
	f_stats->commands_count = command_count;
	bag->mid_bag->instructions->builtin_yes_not = 0;
	is_built_in_set(bag, 1);
	return (f_stats);
}

void	fork_first_parent(int fd[], t_bag *bag, t_var *var, t_fork *f_stats)
{
	close(fd[1]);
	waitpid(f_stats->pid, NULL, 0);
	if (bag->mid_bag->instructions->out_redirect_type
		&& !bag->mid_bag->instructions->builtin_yes_not)
	{
		read_util(bag, fd);
		close(fd[0]);
		bag->mid_bag->instructions->echo_option = 1;
		ft_echo(bag->mid_bag->instructions, var);
	}
}

void	create_fd(t_bag *bag, int fd_2[])
{
	g_env_error = 0;
	bag->mid_bag = bag->mid_bag->next;
	bag->mid_bag->instructions->builtin_yes_not = 0;
	is_built_in_set(bag, 1);
	pipe(fd_2);
}

int	fork_multi_command(t_bag *bag, t_fd *fd, t_fork *f_stats, t_var *var)
{
	if (bag->mid_bag->instructions->in_redirect_type
		|| bag->mid_bag->instructions->heredocs_switch)
	{
		if (in_redirect_fork_util(bag->mid_bag->instructions) == -1)
			return (1);
	}
	else
		dup2(fd->fd[0], STDIN_FILENO);
	if (f_stats->commands_count - f_stats->count > 1
		|| bag->mid_bag->instructions->out_redirect_type)
		dup2(fd->fd_2[1], STDOUT_FILENO);
	return (fork_loop_second_child(bag, fd->fd, fd->fd_2, var));
}

void	fork_multiple_commands(t_bag *bag, t_var *var, int commands_count)
{
	t_fork	*f_stats;
	t_fd	*fd;

	fd = malloc(sizeof(t_fd));
	fd_backup_(fd->fd_backup, 0);
	f_stats = set_signal_fork_multi(bag, commands_count);
	pipe(fd->fd);
	f_stats->pid = fork();
	if (f_stats->pid == 0)
		exit(fork_loop_first_child(bag, fd->fd, var));
	else
		fork_first_parent(fd->fd, bag, var, f_stats);
	while (f_stats->count < f_stats->commands_count)
	{
		create_fd(bag, fd->fd_2);
		f_stats->pid_1 = fork();
		if (f_stats->pid_1 == 0)
			exit (fork_multi_command(bag, fd, f_stats, var));
		else
			fork_second_parent(fd, bag, var, f_stats);
	}
	close(fd->fd[0]);
	fd_backup_(fd->fd_backup, 1);
	set_env_error(bag, f_stats->pid_1_ex_status);
	free(f_stats);
}
