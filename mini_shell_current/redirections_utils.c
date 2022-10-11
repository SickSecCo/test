#include "minishell.h"

void	take_inputs_loop(t_commands *instructions)
{
	int l = 0, count = 0;
	char *prompt = "> ";
	char *s;
	int a = 0;
	char *string;
	int pid, pid_status;
	char *str, *str_1;
	struct sigaction	sig_here_doc;

	sigemptyset(&sig_here_doc.sa_mask);
	sig_here_doc.sa_flags = 0;
	sig_here_doc.sa_sigaction = signal_handler_1;
	sigaction(SIGINT, &sig_here_doc, NULL);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		sig_here_doc.sa_sigaction = signal_handler_4;
		sigaction(SIGINT, &sig_here_doc, NULL);
		while (count < instructions->delimiters_count)
		{
			s = readline(prompt);
			if (!s)
				break ;
			if (ft_strcmp(s, instructions->arr_delimiters[count]) == 0)
			{
				if (instructions->delimiters_count >= 2)
				{
					if (!ft_strcmp(s, instructions->arr_delimiters[instructions->delimiters_count - 2]))
						a++;
				}
				else
					if (!ft_strcmp(s, instructions->arr_delimiters[instructions->delimiters_count - 1]))
						a++;
				count++;
			}
			else if (instructions->delimiters_count < 2 || (instructions->delimiters_count >= 2 && a > 0))
			{
				instructions->heredocs_switch = 1;
				string = ft_strjoin(s, "\n");
				if (strlen(s) <= 0)
				{
					instructions->heredoc_array[instructions->heredoc_string_count] = ft_strdup(string);
					instructions->heredoc_string_count++;
				}
				else
				{
					instructions->heredoc_array[instructions->heredoc_string_count] = ft_strdup(string);
					instructions->heredoc_string_count++;
				}
				free(string);
			}
			free(s);
		}
		instructions->heredoc_fd = open("here_doc.txt", O_RDWR | O_TRUNC | O_CREAT , 0666);
		if (instructions->heredocs_switch)
		{
			str_1 = ft_strdup(instructions->heredoc_array[0]);
			l++;
			while (l < instructions->heredoc_string_count)
			{
				str = ft_strjoin(str_1, instructions->heredoc_array[l]);
				free(str_1);
				str_1 = ft_strdup(str);
				free(str);
				l++;
			}
			write(instructions->heredoc_fd, str_1, ft_strlen(str_1));
			free(str_1);
		}
		close(instructions->heredoc_fd);
		exit(0);
	}
	else
		waitpid(pid, &pid_status, 0);
	printf("PID : %d\n", pid_status);
	// if (pid_status == 0)
	// 	return ;
	prompt = NULL;
	s = NULL;
	//rimetti a posto i segnali;
}

int	take_delimiters(t_commands **instructions, int i)
{
    int j;
    
	i+=2;
	while((*instructions)->argument[i] ==  ' ')
		i++;
	j = i;
	while((*instructions)->argument[i] !=  ' ' && (*instructions)->argument[i] !=  '\0')
		i++;
	(*instructions)->arr_delimiters[(*instructions)->delimiters_count] = ft_substr((*instructions)->argument, j, i - j);
	(*instructions)->delimiters_count++;
	while((*instructions)->argument[i] ==  ' ')
		i++;
	if ((*instructions)->argument[i] !=  '\0')
		i = ft_min_single_file(instructions, i - 1);
	return (i);
}

int	multiple_file(char *input)
{
	int	i;

	i = 0;
	while (input[i] != '\0')
	{
		while (input[i] != '<')
			i++;
		while (input[i] == '<' || input[i] == ' ')
			i++;
		while (input[i] != ' ' && input[i] != '<' && input[i] != '\0')
			i++;
		while (input[i] == ' ')
			i++;
		if (input[i] != '<' && input[i] != '\0')
			return (1);
	}
	return (0);
}

char	*f_arg(char *input, t_commands *instruction)
{
	int		i;
	int		j;
	int		k;
	char	*temp;
	int		l;
	int		x;

	i = 0;
	j = 0;
	l = 0;
	x = 0;
	temp = malloc(ft_strlen(input));
	while (input[i] != '\0')
	{
		k = 0;
		if (input[i] == '>')
		{
			i++;
			if (input[i] == '>')
				i++;
			while (input[i] == ' ')
				i++;
			while(input[i] != ' ' && input[i] != '\0')
				i++;
			// while(input[i] == ' ')
			// 	i++;
			continue ;
		}
		else if (input[i] == '<')
		{
			i++;
			while (input[i] != '>' && input[i] != '\0')
				i++;
			continue ;
		}
		else if (input[i] == '\'')
		{
			instruction->quote_start[l] = i - x;printf("instruction->quote_start[l] : %d\n", instruction->quote_start[l]);
			i++;
			x++;
			while(input[i] != '\'' && input[i] != '\0')
			{
				temp[j] = input[i];
				j++;
				i++;
			}
			instruction->quote_end[l] = i - x - 1;printf("instruction->quote_end[l] : %d\n", instruction->quote_end[l]);
			if (input[i] == '\'')
				i++;
			x++;
			l++;
		}
		else if (input[i] == '\"')
		{
			instruction->quote_start[l] = i - x;printf("instruction->quote_start[l] : %d\n", instruction->quote_start[l]);
			i++;
			x++;
			while(input[i] != '\"' && input[i] != '\0')
			{
				temp[j] = input[i];
				j++;
				i++;
			}
			instruction->quote_end[l] = i - x - 1;printf("instruction->quote_end[l] : %d\n", instruction->quote_end[l]);
			if (input[i] == '\"')
				i++;
			x++;
			l++;
		}
		else if (input[i] == ' ')
		{
			while(input[i + k] == ' ')
				k++;
			if (input[i + k] == '<' || input[i + k] == '>')
			{
				i = i + k;
				continue ;
			}
		}
		temp[j] = input[i];
		j++;
		i++;
	}
	temp[j] = '\0';
	return (temp);
}
