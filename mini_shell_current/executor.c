/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiulian <fgiulian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 19:50:17 by fgiulian          #+#    #+#             */
/*   Updated: 2022/10/20 20:49:50 by fgiulian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*path_find_pwdbased(char *str, t_var *var)
{
	int		y;
	char	*return_str;
	char	*temp;

	y = 0;
	if (!access(str, X_OK))
	{
		return_str = ft_strdup(str);
		return (return_str);
	}
	while (y < var->count)
	{
		if (!ft_strcmp(var->var_name[y], "PWD"))
			break ;
		y++;
	}
	temp = ft_strdup(var->var_value[y]);
	if (!access(ft_strjoin(temp, str), X_OK))
	{
		return_str = ft_strjoin(temp, str);
		free(temp);
		return (return_str);
	}
	free(temp);
	return (NULL);
}

int	path_find_pathbased_util(char **splitted_path, char *str, int count)
{
	char	*temp;

	while (count >= 0)
	{
		temp = ft_strjoin(splitted_path[count], "/");
		if (!access(ft_strjoin(temp, str), X_OK))
		{
			free(temp);
			temp = NULL;
			break ;
		}
		free(temp);
		temp = NULL;
		count--;
	}
	return (count);
}

char	*path_find_pathbased(char **spl, char *str, char *str_, int count)
{
	char	*temp;
	char	*return_str;

	count = path_find_pathbased_util(spl, str, count);
	if (count >= 0)
	{
		temp = ft_strjoin(spl[count], "/");
		return_str = ft_strjoin(temp, str);
		free(str_);
		free(spl);
		free(temp);
		return (return_str);
	}
	else
	{
		free(str_);
		free(spl);
	}
	return (NULL);
}

char	*pathfinder(char *str, t_var *var)
{
	int		count;
	char	**splitted_path;
	char	*str_;
	int		k;
	char	*return_str;

	k = -1;
	count = 0;
	return_str = path_find_pwdbased(str, var);
	if (return_str)
		return (return_str);
	while (++k < var->count)
		if (!ft_strcmp(var->var_name[k], "PATH"))
			break ;
	str_ = ft_strdup(var->var_value[k]);
	splitted_path = split_basic(str_, 58, &count);
	return_str = path_find_pathbased(splitted_path, str, str_, count);
	if (return_str)
		return (return_str);
	return (NULL);
}

int	execute_command(t_bag *bag, t_var *var)
{
	char	*str;

	str = pathfinder(bag->mid_bag->instructions->command, var);
	if (str == NULL)
	{
		printf("%s: command not found\n", bag->mid_bag->instructions->command);
		return (127);
	}
	complete_arr_execve(&bag->mid_bag->instructions);
	execve(str, bag->mid_bag->instructions->arr_execve, var->var_execve);
	printf("%s: command not found\n", bag->mid_bag->instructions->command);
	return (127);
}
