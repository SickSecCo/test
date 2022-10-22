/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiulian <fgiulian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 20:31:44 by fgiulian          #+#    #+#             */
/*   Updated: 2022/10/19 20:31:44 by fgiulian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	complete_arr_execve(t_commands **instructions)
{
	int	k;

	k = 0;
	(*instructions)->arr_execve[k++] = ft_strdup((*instructions)->command);
	(*instructions)->arr_execve = ft_splitto((*instructions)->arr_execve,
			(*instructions)->output_arg, 32, &k);
	(*instructions)->arr_execve_index = k;
	(*instructions)->arr_execve[(*instructions)->arr_execve_index] = NULL;
}

int	fork_single_command(int link[], t_bag *bag, t_var *var)
{
	if (bag->mid_bag->instructions->out_redirect_type)
		dup2(link[1], STDOUT_FILENO);
	close(link[0]);
	close(link[1]);
	if (!ft_strcmp(bag->mid_bag->instructions->command, "pwd")
		|| !ft_strcmp(bag->mid_bag->instructions->command, "env"))
	{
		check_command(bag, var);
		return (0);
	}
	else
		execute_command(bag, var);
	return (127);
}

void	fd_backup_(int fd[], int i)
{
	if (i == 0)
	{
		fd[0] = dup(STDIN_FILENO);
		fd[1] = dup(STDOUT_FILENO);
	}
	else
	{
		dup2(fd[0], STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
	}
}

void	set_signal(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	set_env_error(t_bag *bag, int wstatus)
{
	if (g_env_error == 0
		|| (g_env_error != 130 && g_env_error != 131))
		g_env_error = WEXITSTATUS(wstatus);
	bag->err_env_err = 1;
}
