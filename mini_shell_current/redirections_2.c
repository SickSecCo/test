/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiulian <fgiulian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 19:04:58 by fgiulian          #+#    #+#             */
/*   Updated: 2022/10/21 19:04:58 by fgiulian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_min_if(t_commands *instructions, int **index, char *str)
{
	if (str[(*index)[0]] == '<' && str[(*index)[0] + 1] != '<'
			&& (*index)[2] != 1)
		(*index)[0] = ft_min_single_file(&instructions, (*index)[0],
				instructions->argument);
	else if (str[(*index)[0]] == '<' && str[(*index)[0] + 1] == '<')
	{
		(*index)[2] = 1;
		(*index)[6] = 1;
		(*index)[0] = take_delimiters(&instructions, (*index)[0]);
		instructions->in_redirect_type = 2;
	}
}

void	ft_continue(t_commands *instructions, char *file,
			int **index, char *str)
{
	if ((*index)[3] != 0 && (*index)[4] == 0)
	{
		(*index)[5] = open(file, O_RDWR | O_CREAT | O_TRUNC, 0666);
		close((*index)[5]);
	}
	(*index)[3] = (*index)[0];
	(*index)[4] = 1;
	(*index)[0] += 2;
	while (str[(*index)[0]] == ' ')
		(*index)[0]++;
	(*index)[1] = (*index)[0];
	while (str[(*index)[1]] != ' ' && str[(*index)[1]] != '\0')
		(*index)[1]++;
	file = ft_substr(str, (*index)[0], (*index)[1] - (*index)[0]);
	instructions->file_output = ft_strdup(file);
	instructions->out_redirect_type = 2;
}

void	ft_maj_if(t_commands *instructions, char *file, int **index, char *str)
{
	if (str[(*index)[0]] == '>' && str[(*index)[0] + 1] != '>')
	{
		if ((*index)[3] != 0 && (*index)[4] == 0)
		{
			(*index)[5] = open(file, O_RDWR | O_CREAT | O_TRUNC, 0666);
			close((*index)[5]);
		}
		(*index)[3] = (*index)[0];
		(*index)[4] = 0;
		(*index)[0] += 1;
		while (str[(*index)[0]] == ' ')
			(*index)[0]++;
		(*index)[1] = (*index)[0];
		while (str[(*index)[1]] != ' ' && str[(*index)[1]] != '\0')
			(*index)[1]++;
		file = ft_substr(str, (*index)[0], (*index)[1] - (*index)[0]);
		instructions->file_output = ft_strdup(file);
		instructions->out_redirect_type = 1;
	}
	else
		ft_continue(instructions, file, index, str);
}

void	min_single_file_2(t_commands **instructions, int i, int j, int fd)
{
	if ((*instructions)->file_input)
	{
		free((*instructions)->file_input);
		(*instructions)->file_input = NULL;
	}
	(*instructions)->file_input
		= ft_substr((*instructions)->argument, i, j - i);
	fd = open((*instructions)->file_input, O_RDONLY);
	close(fd);
	if (fd == -1)
		(*instructions)->err_yes_no = 1;
}
