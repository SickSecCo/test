#include "minishell.h"

void    ft_pwd()
{
    char    *path;

    path = malloc(sizeof(char) * 300);
    path = getcwd(path, 300);
    printf("%s\n", path);
	free(path);
}

void    ft_cd(t_tree *mid_bag, t_var *var)
{
	int		i;
	char	*str;

	i = 0;
    if (chdir(mid_bag->instructions->output_arg))
    {
        printf("No such file or directory\n");
		mid_bag->err_env_err = 1;
		env_error = 1;
    }
	else
	{
		while (i < var->count)
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
			i++;
		}
		free(str);
	}
}

void    ft_echo(t_commands *instruction, t_var *var)
{
	char	*echo_print;
	int 	fd;

	if (!ft_strcmp(instruction->command, "echo"))
		echo_print = ft_var_output(instruction->output_arg, instruction, var, 0);
	else
		echo_print = ft_strdup(instruction->output_arg);
	if (instruction->out_redirect_type != 0 && echo_print != NULL)
	{
        if (instruction->out_redirect_type == 1)
		{
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
		else if (instruction->out_redirect_type == 2)
		{
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

void	ft_export(t_var *var, t_tree *bag)
{
	int	i;
	int	j;

	i = 0;
	j = check_argument_export(var, bag);
	while (i < var->count)
	{
		if (!j)
		{
			if (!ft_strcmp(var->name, var->var_name[i]))
			{
				ft_var_change(var, i);
				if (bag->instructions->argument[0] != '\0')
					check_command(bag, var);
				return ;
			}
		}
		else
		{
			bag->err_env_err = 1;
			env_error = 1;
		}
		i++;
	}
	if (!j)
	{
		var->var_name[var->count] = ft_strdup(var->name);
		var->var_value[var->count] = ft_strdup(var->value);
		var->count++;
	}
	if (bag->instructions->argument[0] != '\0')
		check_command(bag, var);
}

void	ft_unset(t_var *var, t_tree *bag)
{
	int		i;

	i = 0;
	while (i < var->count)
	{
		if (!ft_strcmp(var->var_name[i], var->name))
			break ;
		i++;
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
	if (bag->instructions->argument[0] != '\0')
	{
		check_arguments_unset(var, bag);
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
	env_error = 0;
}
