/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiulian <fgiulian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 19:50:44 by fgiulian          #+#    #+#             */
/*   Updated: 2022/10/21 18:12:36 by fgiulian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct sigaction	set_signal_quit_ignore(void)
{
	struct sigaction	sig_here_doc;

	sigemptyset(&sig_here_doc.sa_mask);
	sig_here_doc.sa_flags = 0;
	sig_here_doc.sa_sigaction = signal_handler_1;
	sigaction(SIGINT, &sig_here_doc, NULL);
	signal(SIGQUIT, SIG_IGN);
	return (sig_here_doc);
}

void	take_inputs_util_1(int *count, t_commands *instructions,
				int *a, char *s)
{
	if (instructions->delimiters_count >= 2)
	{
		if (!ft_strcmp(s, instructions->arr_delimiters
				[instructions->delimiters_count - 2]))
			(*a)++;
	}
	else
		if (!ft_strcmp(s, instructions->arr_delimiters
				[instructions->delimiters_count - 1]))
			(*a)++;
	(*count)++;
}

void	take_inputs_util_2(t_commands *instructions, char *s)
{
	char	*string;

	instructions->heredocs_switch = 1;
	string = ft_strjoin(s, "\n");
	if (strlen(s) <= 0)
	{
		instructions->heredoc_array[instructions->heredoc_string_count]
			= ft_strdup(string);
		instructions->heredoc_string_count++;
	}
	else
	{
		instructions->heredoc_array[instructions->heredoc_string_count]
			= ft_strdup(string);
		instructions->heredoc_string_count++;
	}
	free(string);
}

void	take_inputs_loop_util(t_commands *instructions)
{
	int		count;
	int		a;
	char	*s;
	char	*prompt;

	prompt = ft_strdup("> ");
	a = 0;
	count = 0;
	while (count < instructions->delimiters_count)
	{
		s = readline(prompt);
		if (!s)
			break ;
		if (ft_strcmp(s, instructions->arr_delimiters[count]) == 0)
			take_inputs_util_1(&count, instructions, &a, s);
		else if (instructions->delimiters_count < 2
			|| (instructions->delimiters_count >= 2 && a > 0))
			take_inputs_util_2(instructions, s);
		free(s);
	}
}

void	sig_default(void)
{
	struct sigaction	mysig_restore;

	sigemptyset(&mysig_restore.sa_mask);
	mysig_restore.sa_flags = 0;
	mysig_restore.sa_sigaction = signal_handler;
	sigaction(SIGINT, &mysig_restore, NULL);
	signal(SIGQUIT, SIG_IGN);
}
