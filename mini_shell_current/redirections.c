/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiulian <fgiulian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 19:50:47 by fgiulian          #+#    #+#             */
/*   Updated: 2022/10/22 17:54:46 by fgiulian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_handle_heredoc(t_commands *instructions)
{
	instructions->heredoc_array = malloc(sizeof(char *) * 100);
	take_inputs_loop(instructions);
}

int	*ft_arr_init(int *index, t_commands *instructions)
{
	index[0] = 0;
	index[1] = 0;
	index[2] = -1;
	index[3] = 0;
	index[4] = -1;
	index[5] = 0;
	index[6] = 0;
	instructions->quote_count = 0;
	return (index);
}

void	ft_while_go(int **index, t_commands **instructions)
{
	while ((*index)[0] < (*instructions)->quote_end
		[(*instructions)->quote_count]
		&& (*instructions)->argument)
		(*index)[0]++;
	(*instructions)->quote_count++;
}

void	ft_check_redi(t_commands *instructions)
{
	char	*file;
	int		*index;

	file = NULL;
	index = malloc(sizeof(int) * 7);
	index = ft_arr_init(index, instructions);
	while (instructions->argument[index[0]])
	{
		if (index[0] == instructions->quote_start[instructions->quote_count]
			&& instructions->quote_flag[instructions->quote_count] != 0)
			ft_while_go(&index, &instructions);
		if (instructions->argument[index[0]] == '>')
			ft_maj_if(instructions, file, &index, instructions->argument);
		else if (instructions->argument[index[0]] == '<')
			ft_min_if(instructions, &index, instructions->argument);
		index[0]++;
	}
	if (index[6] == 1)
		ft_handle_heredoc(instructions);
	instructions->output_arg = f_arg(instructions->argument, instructions);
	free(file);
	free(index);
	file = NULL;
}

int	ft_min_single_file(t_commands **instructions, int i, char *str)
{
	int		fd;
	int		j;

	i += 1;
	fd = 0;
	while (str[i] != '\0' && str[i] != '<' && str[i] != '>')
	{
		while (str[i] == ' ' || str[i] == '\'' || str[i] == '\"')
			i++;
		j = i;
		while (str[j] != ' ' && str[j] != '\0' && str[j] != '<' && str[j] != '>'
			&& str[j] != '\'' && str[j] != '\"')
			j++;
		if (j != i && (*instructions)->err_yes_no == 0)
			min_single_file_2(instructions, i, j, fd);
		i = j;
		while (str[i] == ' ' || str[i] == '\'' || str[i] == '\"')
			i++;
	}
	(*instructions)->in_redirect_type = 1;
	return (i - 1);
}
