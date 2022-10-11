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

// void	ft_var(t_var *var)
// {
// 	int		i;
// 	char	*temp;
// 	char	*placeholder;

// 	i = 0;
// 	while (i < var->count)
// 	{
// 		if (var->var_name[i][0] != '\0')
// 		{
// 			temp = ft_strjoin(var->var_name[i], "=");
// 			if (!ft_strcmp(var->var_name[i], "SHLVL"))
// 				placeholder = ft_itoa(ft_atoi(var->var_value[i]) + 1);
// 			else
// 				placeholder = ft_strdup(var->var_value[i]);
// 			var->var_execve[i] = ft_strjoin(temp, placeholder);
// 		}
// 		free(temp);
// 		free(placeholder);
// 		i++;
// 	}
// }

int	ft_set_x(t_var *var, int i, char *str)
{
	int		k;
	int		count;
	int		j;
	char	*dest;

	j = 0;
	k = 0;
	while ((str[i + j] >= 48 && str[i + j] <= 57) || (str[i + j] >= 65 && str[i + j] <= 90) || (str[i + j] >= 97 && str[i + j] <= 122))
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
	int		*index;
	char	*dest;

	index = malloc(sizeof(int) * 5);
	index[0] = 0;
	index[1] = 0;
	index[2] = 0;
	index[3] = 0;
	dest = malloc(sizeof(char) * (ft_strlen(instruction->output_arg) + 150));
	while (str[index[0]] != '\0')
	{
		index[4] = 0;
		if ((instruction->quote_flag[index[2]] != 1
				|| index[0] < instruction->quote_start[index[2]])
			&& (str[index[0]] == '$' && str[index[0] + 1] != ' '))
		{
			if (str[index[0]] == '$' && str[index[0] + 1] == '?')
			{
				index[1] += figure_count();
				dest = ft_putnbr(dest, index[1], env_error, 0);
				index[0] += 2;
				index[1]++;
			}
			if (count[index[3]] == var->count)
			{
				if (str[index[0]] == '$' && str[index[0] + 1] == '\0')
				{
					dest[index[1]] = str[index[0]];
					index[1]++;
				}
				index[0]++;
				while ((str[index[0]] >= 48 && str[index[0]] <= 57)
						|| (str[index[0]] >= 65 && str[index[0]] <= 90)
							|| (str[index[0]] >= 97 && str[index[0]] <=122))
					index[0]++;
				if (str[index[0]] == '\0')
					break ;
				else
					index[3]++;
			}
			else
			{
				index[0]++;
				while ((str[index[0]] >= 48 && str[index[0]] <= 57)
						|| (str[index[0]] >= 65 && str[index[0]] <= 90)
							|| (str[index[0]] >= 97 && str[index[0]] <=122))
					index[0]++;
				while (var->var_value[count[index[3]]][index[4]] != '\0')
				{	
					dest[index[1]] = var->var_value[count[index[3]]][index[4]];
					index[1]++;
					index[4]++;
				}
				index[3]++;
			}
			if (index[0] == instruction->quote_end[index[2]] + 1)
				index[2]++;
			continue ;
		}
		dest[index[1]] = str[index[0]];
		index[1]++;
		index[0]++;
	}
	dest[index[1]] = '\0';
	free(index);
	return (dest);
}

char	*ft_var_output(char *str, t_commands *instruction, t_var *var, int i)
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
