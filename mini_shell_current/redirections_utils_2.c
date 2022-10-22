/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiulian <fgiulian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 17:56:51 by fgiulian          #+#    #+#             */
/*   Updated: 2022/10/21 17:56:51 by fgiulian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	take_inputs_loop_util_(t_commands *instructions)
{
	char	*str_1;
	char	*str;
	int		l;

	l = 0;
	str_1 = ft_strdup(instructions->heredoc_array[0]);
	l++;
	while (l < instructions->heredoc_string_count)
	{
		str = ft_strjoin(str_1, instructions->heredoc_array[l]);
		free(str_1);
		str_1 = ft_strdup(str);
		free(str);
		l++;
	}
	write(instructions->heredoc_fd, str_1, ft_strlen(str_1));
	free(str_1);
}

void	take_inputs_loop(t_commands *instructions)
{
	int					pid;
	int					pid_status;
	struct sigaction	sig_;

	sig_ = set_signal_quit_ignore();
	pid = fork();
	if (pid == 0)
	{
		sig_.sa_sigaction = signal_handler_4;
		sigaction(SIGINT, &sig_, NULL);
		take_inputs_loop_util(instructions);
		instructions->heredoc_fd = open("h.txt",
				O_RDWR | O_TRUNC | O_CREAT, 0666);
		if (instructions->heredocs_switch)
			take_inputs_loop_util_(instructions);
		close(instructions->heredoc_fd);
		exit(0);
	}
	else
		waitpid(pid, &pid_status, 0);
	sig_default();
}

int	take_delimiters(t_commands **instructions, int i)
{
	int	j;

	i += 2;
	while ((*instructions)->argument[i] == ' ')
		i++;
	j = i;
	while ((*instructions)->argument[i] != ' '
		&& (*instructions)->argument[i] != '\0')
		i++;
	(*instructions)->arr_delimiters[(*instructions)->delimiters_count]
		= ft_substr((*instructions)->argument, j, i - j);
	(*instructions)->delimiters_count++;
	while ((*instructions)->argument[i] == ' ')
		i++;
	if ((*instructions)->argument[i] != '\0')
		i = ft_min_single_file(instructions, i - 1, (*instructions)->argument);
	return (i);
}

int	multiple_file(char *input)
{
	int	i;

	i = 0;
	while (input[i] != '\0')
	{
		while (input[i] != '<')
			i++;
		while (input[i] == '<' || input[i] == ' ')
			i++;
		while (input[i] != ' ' && input[i] != '<' && input[i] != '\0')
			i++;
		while (input[i] == ' ')
			i++;
		if (input[i] != '<' && input[i] != '\0')
			return (1);
	}
	return (0);
}
