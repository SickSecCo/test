/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiulian <fgiulian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 19:50:02 by fgiulian          #+#    #+#             */
/*   Updated: 2022/10/22 16:52:51 by fgiulian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_cd_util(t_var *var)
{
	char	*str;
	int		i;

	i = -1;
	while (++i < var->count)
	{
		if (!ft_strcmp(var->var_name[i], "PWD"))
		{
			str = ft_strdup(var->var_value[i]);
			var->var_value[i] = getcwd(var->var_value[i], 500);
		}
		else if (!ft_strcmp(var->var_name[i], "OLDPWD"))
		{
			free(var->var_value[i]);
			var->var_value[i] = ft_strdup(str);
		}
	}
	free(str);
}

void	ft_cd(t_bag *bag, t_var *var)
{
	if (chdir(bag->mid_bag->instructions->output_arg))
	{
		printf("No such file or directory\n");
		bag->err_env_err = 1;
		g_env_error = 1;
	}
	else
		ft_cd_util(var);
}

void	ft_echo_util(t_commands *instruction, char *echo_print)
{
	int	fd;

	if (instruction->echo_option == 1)
	{
		fd = open(instruction->file_output, O_RDWR | O_TRUNC | O_CREAT, 0666);
		write(fd, &echo_print[0], ft_strlen(echo_print));
		close(fd);
	}
	else
	{
		fd = open(instruction->file_output, O_RDWR | O_TRUNC | O_CREAT, 0666);
		write(fd, &echo_print[0], ft_strlen(echo_print));
		close(fd);
		fd = open(instruction->file_output, O_RDWR | O_APPEND | O_CREAT, 0666);
		write(fd, "\n", 1);
		close(fd);
	}
}

void	ft_echo_util_1(t_commands *instruction, char *echo_print)
{
	int	fd;

	if (instruction->echo_option == 1)
	{
		fd = open(instruction->file_output, O_RDWR | O_APPEND | O_CREAT, 0666);
		write(fd, &echo_print[0], ft_strlen(echo_print));
		close (fd);
	}
	else
	{
		fd = open(instruction->file_output, O_RDWR | O_APPEND | O_CREAT, 0666);
		write(fd, &echo_print[0], ft_strlen(echo_print));
		close(fd);
		fd = open(instruction->file_output, O_RDWR | O_APPEND | O_CREAT, 0666);
		write(fd, "\n", 1);
		close(fd);
	}
}

void	ft_echo(t_commands *instruction, t_var *var)
{
	char	*echo_print;

	if (!ft_strcmp(instruction->command, "echo"))
		echo_print = ft_var_out(instruction->output_arg, instruction, var, 0);
	else
		echo_print = ft_strdup(instruction->output_arg);
	if (instruction->out_redirect_type != 0 && echo_print != NULL)
	{
		if (instruction->out_redirect_type == 1)
			ft_echo_util(instruction, echo_print);
		else if (instruction->out_redirect_type == 2)
			ft_echo_util_1(instruction, echo_print);
	}
	else
	{
		if (instruction->echo_option == 1)
			printf("%s", echo_print);
		else
			printf("%s\n", echo_print);
	}
	free(echo_print);
}
