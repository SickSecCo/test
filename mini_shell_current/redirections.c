#include "minishell.h"

void	ft_check_redi(t_commands *instructions)
{
	int 	i;
	int 	latest_maj;
	int		j;
	int 	previous_red;
	int		fd;
	int		k;
	int		protec;
	char	*file;

	file = NULL;
	i = 0;
	latest_maj = 0;
	j = 0;
	previous_red = -1;
	k = -1;
	protec = 0;
	instructions->quote_count = 0;
	while (instructions->argument[i])
	{
		if (i == instructions->quote_start[instructions->quote_count] && instructions->quote_flag[instructions->quote_count] != 0)
		{
			while (i < instructions->quote_end[instructions->quote_count] && instructions->argument)
				i++;
			instructions->quote_count++;
		}
		if (instructions->argument[i] == '>' || (instructions->argument[i] == '>' && instructions->argument[i + 1] == '>'))
		{
			if (instructions->argument[i] == '>' && instructions->argument[i + 1] != '>')
			{
				if (latest_maj != 0 && previous_red == 0)
				{
					fd = open(file, O_RDWR | O_CREAT | O_TRUNC, 0666);
					close(fd);
				}
				latest_maj = i;
				previous_red = 0;
				i += 1;
				while (instructions->argument[i] == ' ')
					i++;
				j = i;
				while (instructions->argument[j] != ' ' && instructions->argument[j] != '\0')
					j++;
				file = ft_substr(instructions->argument, i, j - i);
				instructions->file_output = ft_strdup(file);
				instructions->out_redirect_type = 1;
			}
			else
			{
				if (latest_maj != 0 && previous_red == 0)
				{
					fd = open(file, O_RDWR | O_CREAT | O_TRUNC, 0666);
					close(fd);
				}
				latest_maj = i;
				previous_red = 1;
				i += 2;
				while (instructions->argument[i] == ' ')
					i++;
				j = i;
				while (instructions->argument[j] != ' ' && instructions->argument[j] != '\0')
					j++;
				file = ft_substr(instructions->argument, i, j - i);
				instructions->file_output = ft_strdup(file);
				instructions->out_redirect_type = 2;
			}
		}
		else if (instructions->argument[i] == '<' && instructions->argument[i + 1] != '<' && k != 1)
		{
			i = ft_min_single_file(&instructions, i);
		}
		else if (instructions->argument[i] == '<' && instructions->argument[i + 1] == '<')
		{
			k = 1;
			protec = 1;
			i = take_delimiters(&instructions, i);
			instructions->in_redirect_type = 2;
		}
		i++;
	}
	if (protec == 1)
	{
		instructions->heredoc_array = malloc(sizeof(char *) * 100);
		take_inputs_loop(instructions);
		i = 0;
	}
	instructions->output_arg = f_arg(instructions->argument, instructions);
	free(file);
	file = NULL;
}

int	ft_min_multiple_files(t_commands **instructions, int i)
{
	int		j = 0;
	int 	fd;
	int		is_first;
	int		k;
	char	*str;
	
	is_first = 0;
	i += 1;
	k = (*instructions)->arr_file_index;
	while ((*instructions)->argument[i] != '\0' && (*instructions)->argument[i] != '>')
	{
		while ((*instructions)->argument[i] == ' ')
			i++;
		j = i;
		while ((*instructions)->argument[j] != ' ' && (*instructions)->argument[j] != '\0')
			j++;
		if (j != i)
		{
			if (is_first != 0)
				(*instructions)->arr_file[k] = ft_substr((*instructions)->argument, i, j - i);
			if (!is_first && (*instructions)->file_input == (NULL))
			{
				str = ft_substr((*instructions)->argument, i, j - i);
				fd = open(str, O_RDONLY); 
				free(str);
				str = NULL;
				close(fd);
				if (fd == -1)
					(*instructions)->err_yes_no = 1;
				else
					(*instructions)->file_input = ft_substr((*instructions)->argument, i, j - i);
			}
			if (is_first != 0)
				k++;
			is_first += 1;
		}
		printf("%s\n", (*instructions)->file_input);// = ft_substr((*instructions)->argument, i, j - i));
		i = j;
		while ((*instructions)->argument[i] == ' ')
			i++;
		printf("%c------%i------%i\n", (*instructions)->argument[i], i, j);
		if ((*instructions)->argument[i] == '<')
			i++;
		(*instructions)->in_redirect_type = 1;
	}
	(*instructions)->arr_file_index = k;
	return (i - 1);
}

int	ft_min_single_file(t_commands **instructions, int i)
{
	int		fd;
	int		j;

	i += 1;
	// (*instructions)->arr_file_index = 2;
	while ((*instructions)->argument[i] != '\0' && (*instructions)->argument[i] != '<' && (*instructions)->argument[i] != '>')
	{
		while ((*instructions)->argument[i] == ' ' || (*instructions)->argument[i] == '\'' || (*instructions)->argument[i] == '\"')
			i++;
		j = i;
		while ((*instructions)->argument[j] != ' ' && (*instructions)->argument[j] != '\0' && (*instructions)->argument[j] != '<' && (*instructions)->argument[j] != '>'
					&& (*instructions)->argument[j] != '\'' && (*instructions)->argument[j] != '\"')
			j++;
		if (j != i && (*instructions)->err_yes_no == 0)
		{
			if ((*instructions)->file_input)
			{
				free((*instructions)->file_input);
				(*instructions)->file_input = NULL;
			}
			(*instructions)->file_input = ft_substr((*instructions)->argument, i, j - i);
			fd = open((*instructions)->file_input, O_RDONLY); 
			close(fd);
			if (fd == -1)
				(*instructions)->err_yes_no = 1;
			// else
			// {
			// 	if ((*instructions)->arr_file[1])
			// 	{
			// 		free((*instructions)->arr_file[1]);
			// 		(*instructions)->arr_file[1] = NULL;
			// 	}
			// 	(*instructions)->arr_file[1] = ft_substr((*instructions)->argument, i, j - i);
			// }
		}
		i = j;
		while ((*instructions)->argument[i] == ' ' || (*instructions)->argument[i] == '\'' || (*instructions)->argument[i] == '\"')
			i++;
	}
	(*instructions)->in_redirect_type = 1;
	return (i - 1);
}

int	double_min_multiple_files(t_commands **instructions, int i)
{
	int		j = 0;
	int		k, h;
	
	i += 1;
	k = (*instructions)->arr_file_index;
	while ((*instructions)->argument[i] != '\0' && (*instructions)->argument[i] != '<' && (*instructions)->argument[i] != '>')
	{
		h = 0;
		if ((*instructions)->argument[i] == '<' || (*instructions)->argument[i] == '>')
			h = 1;
		while ((*instructions)->argument[i] == ' ')
			i++;
		j = i;
		while ((*instructions)->argument[i] != ' ' && (*instructions)->argument[i] != '\0')
			i++;
		if (j != i)
		{
			(*instructions)->arr_file[k] = ft_substr((*instructions)->argument, j, i - j);
			k++;
		}
		while ((*instructions)->argument[i] == ' ')
			i++;
		if (h == 1)
			(*instructions)->in_redirect_type = 1;
	}
	(*instructions)->arr_file_index = k;
	return (i - 1);
}
