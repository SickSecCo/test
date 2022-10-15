#include "minishell.h"

int env_error;


int	strsplitwrite_(char **str2, char *str, char del, int starter)
{
	int	i;
	int	j;

	i = 0;
	while (str[i] != '\0')
	{
		if (charsep(str[i], del) == 1)
			i++;
		else
		{
			j = 0;
			while (charsep(str[i + j], del) == 0)
			j++;
			str2[starter] = (char *)malloc(sizeof(char) * (j + 1));
			write_oneword(str2[starter], str + i, del);
			i = i + j;
			starter++;
		}
	}
	return (starter);
}

char	**split_basic(char const *s, char c, int *starter)
{
	char	*str;
	int		chars;
	int 	count;
	char	**array;

	count = *starter;
	if (s == NULL)
		return (NULL);
	str = (char *)s;
	chars = count_chars(str, c);
	array = malloc(sizeof(char *) * chars);
	count = strsplitwrite_(array, str, c, count);
	array[count] = NULL;
	*starter = count;
	return (array);
}

void	free_arr(char **arr, int counter)
{
	while(counter >= 0)
	{
		free(arr[counter]);
		counter--;
	}
	free(arr);
}

// /usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/snap/bin
char	*pathfinder(char *str, t_var *var, char *return_str)
{
	int count;
	char **splitted_path;
	char *str_;
	int k;
	int y;
	int z;
	char *temp;
	k = 0;
	y = 0;
	count = 0;
	if (!access(str, X_OK))
	{
		return_str = ft_strdup(str);printf("%s\n", return_str);
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
		return_str = ft_strdup(ft_strjoin(return_str, str));printf("%s\n", return_str);
		return (return_str);
	}
	while (k < var->count)
	{
		if (!ft_strcmp(var->var_name[k], "PATH"))
			break ;
		k++;
	}
	str_ = ft_strdup(var->var_value[k]);
	splitted_path = split_basic(str_, 58, &count);
	z = count;
	while (count >= 0)
	{
		temp = ft_strjoin(splitted_path[count], "/");
		if (!access(ft_strjoin(temp, str), X_OK))
		{
			free(temp);
			break;
		}
		free(temp);
		count--;
	}
	if (count >= 0)
	{
		temp = ft_strjoin(splitted_path[count], "/");
		return_str = ft_strjoin(temp, str); 
		free(str_);
		free_arr(splitted_path, z);
		free(temp);
		return(return_str);
	}
	free(str_);
}

int	execute_command(t_bag *bag, t_var *var)
{
	int p;
	int k;
	k = 0;
	char *str_;
	char *str = pathfinder(bag->mid_bag->instructions->command, var, str_);
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

int	in_redirect_fork_util(t_bag *bag, int fd_[])
{
	if (bag->mid_bag->instructions->file_input || bag->mid_bag->instructions->in_redirect_type != 2)
	{
		fd_[0] = open(bag->mid_bag->instructions->file_input, O_RDONLY, 0666);
		if (fd_[0] > 0)
		{
			dup2(fd_[0], STDIN_FILENO);
			close(fd_[0]);
		}
		else
		{
			printf("%s: No such file or directory\n", bag->mid_bag->instructions->file_input);
			exit(0);
		}
	}
	else if (bag->mid_bag->instructions->in_redirect_type == 2)
	{
		bag->mid_bag->instructions->heredoc_fd = open("here_doc.txt", O_RDONLY, 0666);
		if (bag->mid_bag->instructions->heredoc_fd)
		{
			dup2(bag->mid_bag->instructions->heredoc_fd, STDIN_FILENO);
			close(bag->mid_bag->instructions->heredoc_fd);
		}
	}
}


int	read_util(t_bag *bag, int fd[])
{
	int i;
	char *str;
	char *pipe_fd;

	pipe_fd = malloc(sizeof(char) * 10001);
	i = 1;
	memset(bag->mid_bag->instructions->output_arg, 0, ft_strlen(bag->mid_bag->instructions->output_arg));
	while(i > 0)
	{
		i = read(fd[0], pipe_fd, 10000);
		pipe_fd[i] = '\0';
		if (i > 0)
		{
			str = ft_strjoin(bag->mid_bag->instructions->output_arg, pipe_fd);
			free(bag->mid_bag->instructions->output_arg);
			bag->mid_bag->instructions->output_arg = ft_strdup(str);
			free(str);
		}
	}
	free(pipe_fd);
}


int	read_fork_pipe(t_bag *bag, int fd[], int *already_closed, t_var *var)
{
	check_command(bag->mid_bag, var);
	if (bag->mid_bag->instructions->out_redirect_type)
	{
		close(fd[1]);
		close(STDOUT_FILENO);
		read_util(bag, fd);
		close(fd[0]);
		bag->mid_bag->instructions->echo_option = 1;
		ft_echo(bag->mid_bag->instructions, var);
		*already_closed = 1;
	}
}



int	fork_loop_second_child(t_bag *bag, int fd[], int fd_2[], t_var *var)
{
	int already_closed;
	already_closed = 0;
	complete_arr_execve(&bag->mid_bag->instructions, -1);
	if (is_built_in_check(bag, 0))
		check_command(bag->mid_bag, var);
	if (!ft_strcmp(bag->mid_bag->instructions->command, "env") || !ft_strcmp(bag->mid_bag->instructions->command, "pwd"))
		read_fork_pipe(bag, fd_2, &already_closed, var);
	if (!already_closed)
	{
		close(fd_2[1]);
		close(fd_2[0]);	
	}
	close(fd[1]);
	close(fd[0]);
	if (!bag->mid_bag->instructions->builtin_yes_not)
		execute_command(bag, var);
	return(0);
}


int	fork_loop_first_child(t_bag *bag, int fd[], int fd_[], t_var *var)
{
	int already_closed;
	already_closed = 0;
	if (bag->mid_bag->instructions->in_redirect_type || bag->mid_bag->instructions->heredocs_switch)
		in_redirect_fork_util(bag, fd_);
	complete_arr_execve(&bag->mid_bag->instructions, -1);
	dup2(fd[1], STDOUT_FILENO);
	if (is_built_in_check(bag, 0))
		check_command(bag->mid_bag, var);
	if (!ft_strcmp(bag->mid_bag->instructions->command, "env") || !ft_strcmp(bag->mid_bag->instructions->command, "pwd"))
		read_fork_pipe(bag, fd, &already_closed, var);
	if (!already_closed)
	{
		close(fd[1]);
		close(fd[0]);
	}
	if (!bag->mid_bag->instructions->builtin_yes_not)
		execute_command(bag, var);
	return(0);
}



int	main_loop_fork_multiple_commands(t_bag *bag, t_var *var, int commands_count)
{
	t_fork *f_stats;
	int	fd[2];
	int	fd_[2];
	int	fd_2[2];
	int	fd_backup[2];

	fd_backup[0] = dup(STDOUT_FILENO);
	fd_backup[1] = dup(STDIN_FILENO);
	f_stats = malloc(sizeof(t_fork));
	f_stats->count = 1;
	bag->mid_bag->instructions->builtin_yes_not = 0;
	ft_var(var);
	pipe(fd);
	f_stats->pid = fork();
	is_built_in_set(bag, 1);
	/*DEBUG*/
	dup2(fd_backup[0], STDOUT_FILENO);
	dup2(fd_backup[1], STDIN_FILENO);
	if (f_stats->pid == 0)
		exit(fork_loop_first_child(bag, fd, fd_, var));
	else
	{
		close(fd[1]);
		if (bag->mid_bag->instructions->in_redirect_type)
			close(fd_[1]);
		waitpid(f_stats->pid, &f_stats->pid_ex_status, 0);
		if (bag->mid_bag->instructions->out_redirect_type && !bag->mid_bag->instructions->builtin_yes_not)
		{
			read_util(bag, fd);
			close(fd[0]);
			bag->mid_bag->instructions->echo_option = 1;
			ft_echo(bag->mid_bag->instructions, var);
		}
	}
	while (f_stats->count < commands_count)
	{
		bag->mid_bag->instructions->builtin_yes_not = 0;
		pipe(fd_2);
		bag->mid_bag = bag->mid_bag->next;
		is_built_in_set(bag, 1);
		f_stats->pid_1 = fork();
		if (f_stats->pid_1 == 0)
		{
			if (bag->mid_bag->instructions->in_redirect_type || bag->mid_bag->instructions->heredocs_switch)
				in_redirect_fork_util(bag, fd_);
			else
				dup2(fd[0], STDIN_FILENO);
			if (commands_count - f_stats->count > 1 || bag->mid_bag->instructions->out_redirect_type)
				dup2(fd_2[1], STDOUT_FILENO);
			exit(fork_loop_second_child(bag, fd, fd_2, var));
		}
		else
		{
			close(fd_2[1]);
			waitpid(f_stats->pid_1, &f_stats->pid_1_ex_status, 0);
			if (!bag->mid_bag->instructions->out_redirect_type)
				dup2(fd_2[0], fd[0]);
			if (bag->mid_bag->instructions->in_redirect_type)
				close(fd_[1]);
			if (bag->mid_bag->instructions->out_redirect_type && !bag->mid_bag->instructions->builtin_yes_not)
			{
				read_util(bag, fd_2);
				close(fd_2[0]);
				bag->mid_bag->instructions->echo_option = 1;
				ft_echo(bag->mid_bag->instructions, var);
			}
			else
				close(fd_2[0]);
			f_stats->count++;
		}
	}
	dup2(fd_backup[0], STDOUT_FILENO);
	dup2(fd_backup[1], STDIN_FILENO);
	free(f_stats);
}

int	main_loop_fork(t_bag *bag, t_var *var)
{
	int code;
	struct sigaction	sig_struct;

	sigemptyset(&sig_struct.sa_mask);
	sig_struct.sa_flags = 0;
	sig_struct.sa_sigaction = signal_handler_1;
	sigaction(SIGINT, &sig_struct, NULL);
	sigaction(SIGQUIT, &sig_struct, NULL);
	ft_var(var);
	if (!ft_strcmp(bag->mid_bag->instructions->command, "export")
	|| !ft_strcmp(bag->mid_bag->instructions->command, "echo")
	|| !ft_strcmp(bag->mid_bag->instructions->command, "unset")
	|| !ft_strcmp(bag->mid_bag->instructions->command, "cd"))
	{
		check_command(bag->mid_bag, var);
		return 0;
	}
	code = ft_fork(bag, var);
	if (code != 0 && (bag->mid_bag->instructions->in_redirect_type || bag->mid_bag->instructions->out_redirect_type))
	{
		bag->mid_bag->instructions->echo_option = 1;
		ft_echo(bag->mid_bag->instructions, var);
	}
}

int	main_loop_core(t_bag *bag, t_var *var, char *s)
{
	int commands_count;
	int commands_count2;

	commands_count = 0;
	commands_count2 = commands_count;
	if (check_inputs(var, s))
	{
		var->inputs_count++;
		add_history(s);
	}
	commands_count = parse(bag, s, var);
	commands_count2 = commands_count;
	print(bag->mid_bag, commands_count2);
	if (commands_count == -1)
	{
		loop_free(var, bag, commands_count2);
		return (1);
	}
	else if (!ft_strcmp(bag->mid_bag->instructions->command, "exit"))
	{
		loop_free(var, bag, commands_count2);
		return(-1);
	}
	if (env_error == 130 && bag->mid_bag->instructions->in_redirect_type == 2)
	{
		bag->mid_bag->err_env_err = 1;
		return (0);
	}
	if (bag->mid_bag->pipe_yes_not)
		main_loop_fork_multiple_commands(bag, var, commands_count);
	else
		main_loop_fork(bag, var);
	loop_free(var, bag, commands_count2);
	return (0);
}

void	main_loop(struct sigaction sig, char **envp)
{
	char *prompt;
	char *s;
	t_var	*var;
	t_bag	*bag;
	int	code;

	prompt = ft_strdup("\033[93m\033[1mminishell\033[34m$\033[0m ");
	var = malloc(sizeof(t_var));
	var->array_of_inputs = malloc(sizeof(char *) * 100);
	initialize_var(var, envp);
	var->inputs_count = 0;
	code = 0;
	while (1)
	{
		bag = malloc(sizeof(t_bag));
		bag->mid_bag = malloc(sizeof(t_tree));
		bag->mid_bag->err_env_err = 0;
		bag->mid_bag->pipe_yes_not = 0;
		sigaction(SIGINT, &sig, NULL);
		signal(SIGQUIT, SIG_IGN);
		s = readline(prompt);
		if (!s)
		{
			free_no_input(bag, var, s, prompt);
			/* if (strcmp(getenv("SHLVL"), "1"))
			 	write(1, "\n", 1); da vedere sul computer della 42 */
			write(1, "\n", 1); 
			exit(0);
		}
		if (ft_strlen(s) > 0)
		{
			code = main_loop_core(bag, var, s);
			if (code == -1)
				exit(0);
			else if (code == 1)
			{
				if (bag->mid_bag->err_env_err == 0)
					env_error = 0;
				continue;
			}
		}
		if (bag->mid_bag->err_env_err == 0)
			env_error = 0;
		free(s);
		free(bag->mid_bag);
		free(bag);
	}
}

int main(int argc, char **argv, char **envp)
{
	struct sigaction	mysig;

	sigemptyset(&mysig.sa_mask);
	mysig.sa_flags = 0;
	mysig.sa_sigaction = signal_handler;

	set_termios(1);

	main_loop(mysig, envp);
}



