#include "minishell.h"

int	ft_strcmp(char *s1, char *s2)
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
	printf("\tEXIT DEBUG\n");
	bag = backup;
}

int	strsplitwrite_(char **str2, char *str, char del, int starter)
{
	int	i;
	int	j;
	static int x;
	i = 0;
	while (str[i] != '\0')
	{
		if (charsep(str[i], del) == 1)
			i++;
		else
		{
			x++;
			j = 0;
			while (charsep(str[i + j], del) == 0)
			j++;
			str2[starter] = (char *)malloc(sizeof(char) * (j + 1));
			write_oneword(str2[starter], str + i, del);
			i = i + j;
			starter++;
		}
	}
	return (starter - 1);
}

char	**split_basic(char *s, char c, int *starter)
{
	int		chars;
	int 	count;
	char	**array;
	char *str;

	count = *starter;
	if (s == NULL)
		return (NULL);
	chars = count_chars(s, c);
	array = malloc(sizeof(char *) * (chars));
	count = strsplitwrite_(array, s, c, count);
	*starter = count;
	return (array);
}
