/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiulian <fgiulian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 16:22:39 by fgiulian          #+#    #+#             */
/*   Updated: 2022/10/06 16:22:39 by fgiulian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_var(t_var *var)
{
	int		i;
	char	*temp;
	char	*placeholder;

	i = 0;
	while (i < var->count)
	{
		temp = ft_strjoin(var->var_name[i], "=");
		if (!ft_strcmp(var->var_name[i], "SHLVL"))
			placeholder = ft_itoa(ft_atoi(var->var_value[i]) + 1);
		else
			placeholder = ft_strdup(var->var_value[i]);
		var->var_execve[i] = ft_strjoin(temp, placeholder);
		free(temp);
		free(placeholder);
		i++;
	}
}
