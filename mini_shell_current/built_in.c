/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiulian <fgiulian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 20:09:51 by fgiulian          #+#    #+#             */
/*   Updated: 2022/10/19 20:09:51 by fgiulian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_export_util(t_var *var, t_bag *bag, int i, int j)
{
	while (i < var->count)
	{
		if (!j)
		{
			if (!ft_strcmp(var->name, var->var_name[i]))
			{
				ft_var_change(var, i);
				if (bag->mid_bag->instructions->argument[0] != '\0')
					check_command(bag, var);
				return (-1);
			}
		}
		else
		{
			bag->err_env_err = 1;
			g_env_error = 1;
		}
		i++;
	}
	return (i);
}

void	ft_export(t_var *var, t_bag *bag)
{
	int	i;
	int	j;

	i = 0;
	j = check_argument_export(var, bag->mid_bag);
	i = ft_export_util(var, bag, i, j);
	if (i == -1)
		return ;
	if (!j)
	{
		var->var_name[var->count] = ft_strdup(var->name);
		var->var_value[var->count] = ft_strdup(var->value);
		var->count++;
	}
	if (bag->mid_bag->instructions->argument[0] != '\0')
		check_command(bag, var);
}

void	ft_unset(t_var *var, t_bag *bag)
{
	int		i;

	i = -1;
	while (++i < var->count)
	{
		if (!ft_strcmp(var->var_name[i], var->name))
			break ;
	}
	if (i != var->count)
	{
		free(var->var_name[i]);
		free(var->var_value[i]);
		var->count--;
		var->var_name[i] = ft_strdup(var->var_name[var->count]);
		var->var_value[i] = ft_strdup(var->var_value[var->count]);
		free(var->var_name[var->count]);
		free(var->var_value[var->count]);
		var->var_name[var->count] = NULL;
		var->var_value[var->count] = NULL;
	}
	if (bag->mid_bag->instructions->argument[0] != '\0')
	{
		check_arguments_unset(var, bag->mid_bag);
		check_command(bag, var);
	}
}

void	ft_env(t_var *var)
{
	int	i;

	i = 0;
	while (i < var->count)
	{
		if (var->var_name[i][0] != '\0')
			printf("%s=%s\n", var->var_name[i], var->var_value[i]);
		i++;
	}
	g_env_error = 0;
}

void	ft_pwd(void)
{
	char	*path;

	path = malloc(sizeof(char) * 300);
	path = getcwd(path, 300);
	printf("%s\n", path);
	free(path);
}
