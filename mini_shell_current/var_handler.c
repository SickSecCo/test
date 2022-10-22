/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiulian <fgiulian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 16:31:10 by fgiulian          #+#    #+#             */
/*   Updated: 2022/10/06 16:31:10 by fgiulian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_set_x(t_var *var, int i, char *str)
{
	int		k;
	int		count;
	int		j;
	char	*dest;

	j = 0;
	k = 0;
	while ((str[i + j] >= 48 && str[i + j] <= 57)
		|| (str[i + j] >= 65 && str[i + j] <= 90)
		|| (str[i + j] >= 97 && str[i + j] <= 122))
		j++;
	dest = ft_substr(str, i, j);
	while (k < var->count)
	{
		if (!ft_strcmp(dest, var->var_name[k]))
		{
			count = k;
			break ;
		}
		else if (k + 1 == var->count)
			count = k + 1;
		k++;
	}
	free(dest);
	return (count);
}

char	*ft_var2(t_var *var, t_commands *instruction, char *str, int *count)
{
	t_handler	*han;
	char		*dest;

	han = malloc(sizeof(t_handler));
	init_handle(&han, instruction);
	while (str[han->index[0]] != '\0')
	{
		han->index[4] = 0;
		if ((instruction->quote_flag[han->index[2]] != 1
				|| han->index[0] < instruction->quote_start[han->index[2]])
			&& (str[han->index[0]] == '$' && str[han->index[0] + 1] != ' '))
		{
			ft_var_while(&han, str, var, count);
			if (han->index[0] == instruction->quote_end[han->index[2]] + 1)
				han->index[2]++;
			continue ;
		}
		ft_advance(&han, str, instruction);
	}
	han->dest[han->index[1]] = '\0';
	dest = ft_strdup(han->dest);
	free(han->dest);
	free(han->index);
	free(han);
	return (dest);
}

char	*ft_var_out(char *str, t_commands *instruction, t_var *var, int i)
{
	char	*dest;
	int		*count;
	int		x;
	int		y;

	x = 0;
	y = 0;
	count = malloc(sizeof(int) * 1000);
	while (str[i] != '\0')
	{
		if (instruction->quote_flag[y] != 1 || i < instruction->quote_start[y])
		{
			if (str[i] == '$' && str[i + 1] != ' ')
				count[x++] = ft_set_x(var, i + 1, str);
		}
		if (i == instruction->quote_end[y])
			y++;
		i++;
	}
	dest = ft_var2(var, instruction, str, count);
	free(count);
	count = NULL;
	return (dest);
}

void	ft_var_change(t_var *var, int i)
{
	free(var->var_value[i]);
	var->var_value[i] = ft_strdup(var->value);
}

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
	var->var_execve[i] = NULL;
}
