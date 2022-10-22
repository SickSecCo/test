/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiulian <fgiulian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 19:50:13 by fgiulian          #+#    #+#             */
/*   Updated: 2022/10/22 16:53:12 by fgiulian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	in_redirect_fork_util(t_commands *instructions)
{
	int	fd_;

	if (instructions->file_input || instructions->in_redirect_type != 2)
	{
		fd_ = open(instructions->file_input, O_RDONLY, 0666);
		if (fd_ > 0)
		{
			dup2(fd_, STDIN_FILENO);
			close(fd_);
		}
		else
		{
			printf("%s: No such file or directory\n", instructions->file_input);
			g_env_error = 1;
			return (-1);
		}
	}
	else if (instructions->in_redirect_type == 2)
	{
		instructions->heredoc_fd = open("h.txt", O_RDONLY, 0666);
		if (instructions->heredoc_fd)
			dup2(instructions->heredoc_fd, STDIN_FILENO);
		close(instructions->heredoc_fd);
	}
	return (0);
}

void	read_util(t_bag *bag, int fd[])
{
	int		i;
	char	*str;
	char	*pipe_fd;

	pipe_fd = malloc(sizeof(char) * 10001);
	i = 1;
	ft_memset(bag->mid_bag->instructions->output_arg, 0,
		ft_strlen(bag->mid_bag->instructions->output_arg));
	while (i > 0)
	{
		i = read(fd[0], pipe_fd, 10000);
		pipe_fd[i] = '\0';
		if (i > 0)
		{
			str = ft_strjoin(bag->mid_bag->instructions->output_arg, pipe_fd);
			free(bag->mid_bag->instructions->output_arg);
			bag->mid_bag->instructions->output_arg = ft_strdup(str);
			free(str);
		}
	}
	free(pipe_fd);
}

void	read_fork_pipe(t_bag *bag, int fd[], int *already_closed, t_var *var)
{
	check_command(bag, var);
	if (bag->mid_bag->instructions->out_redirect_type)
	{
		close(fd[1]);
		close(STDOUT_FILENO);
		read_util(bag, fd);
		close(fd[0]);
		bag->mid_bag->instructions->echo_option = 1;
		ft_echo(bag->mid_bag->instructions, var);
		*already_closed = 1;
	}
}
