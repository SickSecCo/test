/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiulian <fgiulian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 16:27:49 by fgiulian          #+#    #+#             */
/*   Updated: 2022/10/20 17:38:46 by fgiulian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	initialize_var(t_var *var, char **envp)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (envp[i] && envp[i][0] != '\0')
		i++;
	var->var_name = malloc(sizeof(char *) * 1500);
	var->var_value = malloc(sizeof(char *) * 1500);
	var->count = i;
	i = 0;
	while (i < var->count)
	{
		j = 0;
		while (envp[i][j] != '=' && envp[i][j] != '\0')
			j++;
		var->var_name[i] = ft_substr(envp[i], 0, j);
		var->var_value[i] = ft_substr(envp[i], j + 1,
				ft_strlen(envp[i]) - (j + 1));
		i++;
	}
}

t_tree	*initialize_instructions(t_tree *bag)
{
	bag->instructions = malloc(sizeof(t_commands));
	bag->instructions->file_output = (NULL);
	bag->instructions->file_input = (NULL);
	bag->instructions->out_redirect_type = 0;
	bag->instructions->in_redirect_type = 0;
	bag->instructions->arr_file_index = 0;
	bag->instructions->err_yes_no = 0;
	bag->instructions->arr_file = malloc(sizeof(char *) * 100);
	bag->instructions->arr_execve_index = 0;
	bag->instructions->arr_execve = malloc(sizeof(char *) * 100);
	bag->instructions->delimiters_count = 0;
	bag->instructions->heredoc_string_count = 0;
	bag->instructions->echo_option = 0;
	bag->instructions->quote_end = malloc(sizeof(int) * 100);
	bag->instructions->quote_flag = malloc(sizeof(int) * 100);
	bag->instructions->quote_start = malloc(sizeof(int) * 100);
	bag->instructions->quote_count = 0;
	bag->instructions->heredocs_switch = 0;
	bag->instructions->arr_delimiters = malloc(sizeof(char *) * 100);
	return (bag);
}
