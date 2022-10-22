/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_utils_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiulian <fgiulian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 17:12:14 by fgiulian          #+#    #+#             */
/*   Updated: 2022/10/21 17:12:14 by fgiulian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_name(char *dest, t_var *var, int *index, int *count)
{
	while (var->var_value[count[index[3]]][index[4]] != '\0')
	{	
		dest[index[1]] = var->var_value[count[index[3]]][index[4]];
		index[1]++;
		index[4]++;
	}
	index[3]++;
}

void	ft_var_while(t_handler **handle, char *str, t_var *var, int *count)
{
	if (str[(*handle)->index[0]] == '$' && str[(*handle)->index[0] + 1] == '?')
		ft_int_handle((*handle)->dest, (*handle)->index);
	else if (count[(*handle)->index[3]] == var->count)
		ft_no_name((*handle)->dest, str, (*handle)->index);
	else
	{
		ft_go(str, (*handle)->index);
		ft_name((*handle)->dest, var, (*handle)->index, count);
	}
}
