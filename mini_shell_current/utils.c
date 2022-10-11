#include "minishell.h"

void	signal_handler_4(int code, siginfo_t *siginfo, void *k)
{
	exit(0);
}

void	signal_handler(int code, siginfo_t *siginfo, void *k)
{
	if(code == SIGQUIT)
	{
		rl_on_new_line();
		rl_redisplay();
	}
	else if (code == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		env_error = 130;
	}
}

void	signal_handler_1(int code, siginfo_t *siginfo, void *k)
{
	if (code == SIGINT)
		env_error = 130;
	else
		env_error = 131;
	write(1, "\n", 1);
}

void	set_termios(int code)
{
	struct termios			new;
	static struct termios	old;

	if (code == 1)
	{
		tcgetattr(STDIN_FILENO, &old);
		new = old;
		new.c_lflag &= ~(ECHOCTL);
		tcsetattr(STDIN_FILENO, TCSANOW, &new);
	}
	else
		tcsetattr(STDIN_FILENO, TCSANOW, &old);
}

int		figure_count(void)
{
	int k;
	int	j;

	k = env_error;
	j = 0;
	while (k / 10 > 0)
	{
		j++;
		k = k / 10;
	}
	return (j);
}

char	*ft_putnbr(char *str, int j, int k, int i)
{
	if (k >= 10)
	{
		ft_putnbr(str, j, k / 10, i - 1);
		str[j + i] = k % 10 + 48;
	}
	else
		str[j + i] = k + 48;
	return (str);
}

int		check_inputs(t_var *var, char *input)
{
	static int	i;
	if (var->inputs_count == 0)
		var->array_of_inputs[var->inputs_count] = ft_substr(input, 0, ft_strlen(input));
	else
	{
		if(!ft_strcmp(input, var->array_of_inputs[i]))
			return(0); 
		var->array_of_inputs[var->inputs_count] = ft_substr(input, 0, ft_strlen(input));
		i++;
	}
	return (1);
}

int     ft_strcmp_mini(char *str1, char *str2)
{
    int i;

    i = 0;
    while (str1[i] != '\0')
    {
        if (!ft_strcmp(&str1[i], str2))
           	return (0);
        i++;
    }
    return(1);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int i;

	i = 0;
	while (s1[i] != '\0' && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void print(t_tree *bag, int commands_count)		//DEBUG
{
	t_tree *backup = bag;
	static int x;
	printf("\tENTER DEBUG\n");
	while (commands_count > 0){
		printf("PIPE: %d\n", x++);
		printf("COMMAND : %s\n", bag->instructions->command);
		printf("ARGUMENT : %s\n", bag->instructions->argument);
		// printf("RAW_COMMAND : %s\n", bag->raw_command);
		printf("OUTPUT_ARGUMENT : %s\n", bag->instructions->output_arg);
		printf("FILE OUTPUT: %s\n", bag->instructions->file_output);
		printf("FILE INPUT: %s\n", bag->instructions->file_input);
		printf("OUTPUT TYPE: %d\n", bag->instructions->out_redirect_type);
		printf("INPUT TYPE: %d\n", bag->instructions->in_redirect_type);
		int j = 0;
		printf("MAX ARRAY INDEX: %d\n", bag->instructions->arr_file_index);
		while (j < bag->instructions->arr_file_index)
		{
			printf("FILE NUMERO %d: %s\n", j, bag->instructions->arr_file[j]);
			j++;
		}
		j = 0;
		printf("heredoc_string_count: %d\n", bag->instructions->heredoc_string_count);
		if (bag->instructions->heredoc_string_count != 0)
		{
			while (j < bag->instructions->heredoc_string_count)
			{
				printf("WORDS : %s\n", bag->instructions->heredoc_array[j]);
				j++;
			}		
		}
		bag = bag->next;
		commands_count--;
	}
	printf("\tEXIT DEBUG\n");
	bag = backup;
}
