/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiulian <fgiulian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 20:21:22 by fgiulian          #+#    #+#             */
/*   Updated: 2022/10/19 20:21:22 by fgiulian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	is_built_in_set(t_bag *bag, int code)
{
	if (code == 1)
	{
		if (!ft_strcmp(bag->mid_bag->instructions->command, "export")
			|| !ft_strcmp(bag->mid_bag->instructions->command, "echo")
			|| !ft_strcmp(bag->mid_bag->instructions->command, "unset")
			|| !ft_strcmp(bag->mid_bag->instructions->command, "cd")
			|| !ft_strcmp(bag->mid_bag->instructions->command, "env")
			|| !ft_strcmp(bag->mid_bag->instructions->command, "pwd"))
				bag->mid_bag->instructions->builtin_yes_not = 1;
		else
			bag->mid_bag->instructions->builtin_yes_not = 0;
	}
	else
	{
		if (!ft_strcmp(bag->mid_bag->instructions->command, "export")
			|| !ft_strcmp(bag->mid_bag->instructions->command, "echo")
			|| !ft_strcmp(bag->mid_bag->instructions->command, "unset")
			|| !ft_strcmp(bag->mid_bag->instructions->command, "cd"))
				bag->mid_bag->instructions->builtin_yes_not = 1;
		else
			bag->mid_bag->instructions->builtin_yes_not = 0;
	}
}

int	is_built_in_check(t_bag *bag, int code)
{
	if (code == 1)
	{
		if (!ft_strcmp(bag->mid_bag->instructions->command, "export")
			|| !ft_strcmp(bag->mid_bag->instructions->command, "echo")
			|| !ft_strcmp(bag->mid_bag->instructions->command, "unset")
			|| !ft_strcmp(bag->mid_bag->instructions->command, "cd")
			|| !ft_strcmp(bag->mid_bag->instructions->command, "env")
			|| !ft_strcmp(bag->mid_bag->instructions->command, "pwd"))
			return (1);
		else
			return (0);
	}
	else
	{
		if (!ft_strcmp(bag->mid_bag->instructions->command, "export")
			|| !ft_strcmp(bag->mid_bag->instructions->command, "echo")
			|| !ft_strcmp(bag->mid_bag->instructions->command, "unset")
			|| !ft_strcmp(bag->mid_bag->instructions->command, "cd"))
			return (1);
		else
			return (0);
	}
}
