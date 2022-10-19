#include "minishell.h"

char	*path_find_pwdbased(char *str, t_var *var)
{
	int y;
	char *return_str;

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
	return_str = ft_strdup(var->var_value[y]);
	if (!access(ft_strjoin(return_str, str), X_OK))
	{
		return_str = ft_strjoin(return_str, str);
		return (return_str);
	}
	return (NULL);
}

int	path_find_pathbased_util(char **splitted_path, char *str, int count)
{
	char *temp;
	while (count >= 0)
	{
		temp = ft_strjoin(splitted_path[count], "/");
		if (!access(ft_strjoin(temp, str), X_OK))
		{
			free(temp);
			temp = NULL;
			break;
		}
		free(temp);
		temp = NULL;
		count--;
	}
	return (count);
}

char	*path_find_pathbased(char **splitted_path, char *str, char *str_, int count)
{
	char *temp;
	char *return_str;
	count = path_find_pathbased_util(splitted_path, str, count);
	if (count >= 0)
	{
		temp = ft_strjoin(splitted_path[count], "/");
		return_str = ft_strjoin(temp, str); 
		free(str_);
		free(splitted_path);
		free(temp);
		return(return_str);
	}
	else
	{
		free(str_);
		free(splitted_path);
	}
	return (NULL);
}


char	*pathfinder(char *str, t_var *var, char *return_str)
{
	int count;
	char **splitted_path;
	char *str_;
	int k;
	
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
	int p;
	int k;
	char *str_;
	char *str;
	
	k = 0;
	str = pathfinder(bag->mid_bag->instructions->command, var, str_);
	if (str == NULL)
	{
		printf("%s: command not found_\n", bag->mid_bag->instructions->command);
		return (127);
	}
	complete_arr_execve(&bag->mid_bag->instructions, str);
	execve(str, bag->mid_bag->instructions->arr_execve, var->var_execve);
	printf("%s: command not found\n", bag->mid_bag->instructions->command);
	return (127);
}

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
			bag->mid_bag->instructions->builtin_yes_not =  0;
	}
	else
	{
		if (!ft_strcmp(bag->mid_bag->instructions->command, "export")
		|| !ft_strcmp(bag->mid_bag->instructions->command, "echo")
		|| !ft_strcmp(bag->mid_bag->instructions->command, "unset")
		|| !ft_strcmp(bag->mid_bag->instructions->command, "cd"))
			bag->mid_bag->instructions->builtin_yes_not = 1;
		else
			bag->mid_bag->instructions->builtin_yes_not =  0;
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

