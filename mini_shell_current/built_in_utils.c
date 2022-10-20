/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiulian <fgiulian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 19:50:06 by fgiulian          #+#    #+#             */
/*   Updated: 2022/10/19 20:57:53 by fgiulian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_argument_export_exit(t_tree *bag, char *string, int i)
{
	while (string[i] != ' ' && string[i] != '\0')
		write(1, &string[i++], 1);
	while (string[i] == ' ')
		i++;
	printf("%s\n", ": not a valid identifier");
	bag->instructions->argument = ft_substr(string, i, ft_strlen(string) - i);
	free(string);
	return (-1);
}

int	ft_check_arg_exp_util_1(char *string, int i, t_var *var, t_tree *bag)
{
	int	k;

	k = ++i;
	while (string[i] != '\0')
	{
		if (string[i] == '(' || string[i] == ')')
			return (check_argument_export_exit(bag, string, i));
		i++;
	}
	i = k;
	while (string[i] != ' ' && string[i] != '\0')
		i++;
	if (var->value)
	{
		free(var->value);
		var->value = NULL;
	}
	var->value = ft_substr(string, k, i - k);
	while (string[i] == ' ')
		i++;
	return (i);
}

int	ft_check_arg_exp_util(char *string, int i, t_var *var, t_tree *bag)
{
	while (string[i] != '=' && string[i] != '\0')
	{
		if (!((string[0] >= 65 && string[0] <= 90)
				|| (string[0] >= 97 && string[0] <= 122) || string[0] == '_'))
			return (check_argument_export_exit(bag, string, i));
		if (!((string[i] >= 65 && string[i] <= 90)
				|| (string[i] >= 97 && string[i] <= 122)
				|| (string[i] >= 48 && string[i] <= 57)
				|| string[i] == '_') && i > 0)
			return (check_argument_export_exit(bag, string, i));
		i++;
	}
	if (i == 0)
		return (check_argument_export_exit(bag, string, i));
	if (var->name)
	{
		free(var->name);
		var->name = NULL;
	}
	var->name = ft_substr(string, 0, i);
	if (string[i + 1] == ' ')
		return (check_argument_export_exit(bag, string, i + 2));
	return (i);
}

int	check_argument_export(t_var *var, t_tree *bag)
{
	int		i;
	char	*string;

	i = 0;
	string = ft_strdup(bag->instructions->argument);
	free(bag->instructions->argument);
	if (!string)
		return (1);
	i = ft_check_arg_exp_util(string, i, var, bag);
	if (i == -1)
		return (1);
	i = ft_check_arg_exp_util_1(string, i, var, bag);
	if (i == -1)
		return (1);
	bag->instructions->argument = ft_substr(string, i, ft_strlen(string) - i);
	free(string);
	string = NULL;
	return (0);
}

void	check_arguments_unset(t_var *var, t_tree *bag)
{
	int		i;
	int		k;
	char	*string;

	i = 0;
	k = 0;
	string = ft_strdup(bag->instructions->argument);
	free(bag->instructions->argument);
	while (string[i] != '\0' && string[i] != ' ')
		i++;
	if (var->name)
	{
		free(var->name);
		var->name = NULL;
	}
	var->name = ft_substr(string, k, i);
	while (string[i] == ' ')
		i++;
	bag->instructions->argument = ft_substr(string, i, ft_strlen(string) - i);
	free(string);
	string = NULL;
}
