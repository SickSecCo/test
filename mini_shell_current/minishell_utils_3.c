/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils_3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiulian <fgiulian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 18:50:56 by fgiulian          #+#    #+#             */
/*   Updated: 2022/10/22 18:50:56 by fgiulian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fork_second_parent(t_fd *fd, t_bag *bag, t_var *var, t_fork *f_stats)
{
	close(fd->fd_2[1]);
	waitpid(f_stats->pid_1, &f_stats->pid_1_ex_status, 0);
	if (!bag->mid_bag->instructions->out_redirect_type)
		dup2(fd->fd_2[0], fd->fd[0]);
	close(fd->fd_2[0]);
	if (bag->mid_bag->instructions->out_redirect_type
		&& !bag->mid_bag->instructions->builtin_yes_not)
	{
		read_util(bag, fd->fd);
		bag->mid_bag->instructions->echo_option = 1;
		ft_echo(bag->mid_bag->instructions, var);
	}
	f_stats->count++;
}
