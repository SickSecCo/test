/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiulian <fgiulian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 19:50:21 by fgiulian          #+#    #+#             */
/*   Updated: 2022/10/19 19:50:21 by fgiulian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_fork(t_bag *bag, t_var *var)
{
	int	pid;
	int	wstatus;
	int	link[2];
	int	fd_backup[2];

	fd_backup_(fd_backup, 0);
	if (!ft_strcmp_mini(bag->mid_bag->instructions->command, "minishell"))
		set_signal();
	if (bag->mid_bag->instructions->in_redirect_type
		|| bag->mid_bag->instructions->heredocs_switch)
		if (in_redirect_fork_util(bag->mid_bag->instructions) == -1)
			return (1);
	pipe(link);
	pid = fork();
	if (pid == 0)
		exit (fork_single_command(link, bag, var));
	close(link[1]);
	read_util(bag, link);
	close(link[0]);
	waitpid(pid, &wstatus, 0);
	set_env_error(bag, wstatus);
	fd_backup_(fd_backup, 1);
	return (0);
}

int	fork_loop_second_child(t_bag *bag, int fd[], int fd_2[], t_var *var)
{
	int	already_closed;

	already_closed = 0;
	if (is_built_in_check(bag, 0))
		check_command(bag, var);
	if (!ft_strcmp(bag->mid_bag->instructions->command, "env")
		|| !ft_strcmp(bag->mid_bag->instructions->command, "pwd"))
		read_fork_pipe(bag, fd_2, &already_closed, var);
	if (!already_closed)
	{
		close(fd_2[1]);
		close(fd_2[0]);
	}
	close(fd[1]);
	close(fd[0]);
	if (bag->mid_bag->instructions->builtin_yes_not)
	{
		if (g_env_error == 1)
			return (1);
		return (0);
	}
	if (!bag->mid_bag->instructions->builtin_yes_not)
		execute_command(bag, var);
	return (127);
}

int	fork_loop_first_child(t_bag *bag, int fd[], t_var *var)
{
	int	already_closed;

	already_closed = 0;
	if (bag->mid_bag->instructions->in_redirect_type
		|| bag->mid_bag->instructions->heredocs_switch)
		if (in_redirect_fork_util(bag->mid_bag->instructions) == -1)
			return (1);
	dup2(fd[1], STDOUT_FILENO);
	if (is_built_in_check(bag, 0))
		check_command(bag, var);
	if (!ft_strcmp(bag->mid_bag->instructions->command, "env")
		|| !ft_strcmp(bag->mid_bag->instructions->command, "pwd"))
		read_fork_pipe(bag, fd, &already_closed, var);
	if (!already_closed)
	{
		close(fd[1]);
		close(fd[0]);
	}
	if (bag->mid_bag->instructions->builtin_yes_not)
		return (0);
	if (!bag->mid_bag->instructions->builtin_yes_not)
		execute_command(bag, var);
	return (127);
}
