#include "minishell.h"

int env_error;


int	is_built_in(char *command)
{
	if (!ft_strcmp(command, "export")
		|| !ft_strcmp(command, "echo")
		|| !ft_strcmp(command, "unset")
		|| !ft_strcmp(command, "cd")
		|| !ft_strcmp(command, "env")
		|| !ft_strcmp(command, "pwd"))
		return 1;
	else
		return 0;
}


int	fork_loop_first_child(t_bag *bag, int fd[], int fd_[], int builtin_yes_not, t_var *var)
{
	int i;
	char *str, *pipe_fd;
	if (bag->mid_bag->instructions->in_redirect_type || bag->mid_bag->instructions->heredocs_switch)
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
		complete_arr_execve(&bag->mid_bag->instructions, -1);
		dup2(fd[1], STDOUT_FILENO);
		if (!ft_strcmp(bag->mid_bag->instructions->command, "export")
		|| !ft_strcmp(bag->mid_bag->instructions->command, "echo")
		|| !ft_strcmp(bag->mid_bag->instructions->command, "unset")
		|| !ft_strcmp(bag->mid_bag->instructions->command, "cd"))
			check_command(bag->mid_bag, var);
		if (!ft_strcmp(bag->mid_bag->instructions->command, "env") || !ft_strcmp(bag->mid_bag->instructions->command, "pwd"))
		{
			i = 1;
			check_command(bag->mid_bag, var);
			if (bag->mid_bag->instructions->out_redirect_type)
			{
				close(fd[1]);
				close(STDOUT_FILENO);
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
				close(fd[0]);
				bag->mid_bag->instructions->echo_option = 1;
				ft_echo(bag->mid_bag->instructions, var);
			}
			else
			{
				close(fd[1]);
				close(fd[0]);
			}
		}
		else
		{
			close(fd[1]);
			close(fd[0]);
		}
		if (!builtin_yes_not)
		{
			execve(ft_strjoin("/bin/", bag->mid_bag->instructions->command), bag->mid_bag->instructions->arr_execve, var->var_execve);
			//
		}
		return(0);
}

int	main_loop_fork_multiple_commands(t_bag *bag, t_var *var, int commands_count)
{
	int	fd[2];
	int fd_[2];
	int fd_redirect[2];
	int pid;
	int pid_1, pid_2;
	int pid_ex_status, pid_1_ex_status;
	int count = 1;
	int fd_2[2];
	int	i;
	int builtin_yes_not;
	char *str, *pipe_fd;
	int fd_backup;

	pipe_fd = malloc(sizeof(char) * 10001);
	fd_backup = dup(STDOUT_FILENO);
	builtin_yes_not = 0;
	i = 1;
	ft_var(var);
	pipe(fd);
	pid = fork();
	if (is_built_in(bag->mid_bag->instructions->command))
		builtin_yes_not = 1;
	if (pid == 0)
	{
		if (fork_loop_first_child(bag, fd, fd_, builtin_yes_not, var) == 0)
			exit(0);
	}
	else
	{
		close(fd[1]);
		close(fd_[1]);
		waitpid(pid, &pid_ex_status, 0);
		if (bag->mid_bag->instructions->out_redirect_type && !builtin_yes_not)
		{
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
			close(fd[0]);
			bag->mid_bag->instructions->echo_option = 1;
			ft_echo(bag->mid_bag->instructions, var);
		}
	}
	while (count < commands_count)
	{
		builtin_yes_not = 0;
		pipe(fd_2);
		bag->mid_bag = bag->mid_bag->next;
		if (!ft_strcmp(bag->mid_bag->instructions->command, "export")
		|| !ft_strcmp(bag->mid_bag->instructions->command, "echo")
		|| !ft_strcmp(bag->mid_bag->instructions->command, "unset")
		|| !ft_strcmp(bag->mid_bag->instructions->command, "cd")
		|| !ft_strcmp(bag->mid_bag->instructions->command, "env")
		|| !ft_strcmp(bag->mid_bag->instructions->command, "pwd"))
			builtin_yes_not = 1;
		pid_1 = fork();
		if (pid_1 == 0)
		{
			if (bag->mid_bag->instructions->in_redirect_type || bag->mid_bag->instructions->heredocs_switch)
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
			else
				dup2(fd[0], STDIN_FILENO);
			if (commands_count - count > 1)
				dup2(fd_2[1], STDOUT_FILENO);
			complete_arr_execve(&bag->mid_bag->instructions, -1);
			if (!ft_strcmp(bag->mid_bag->instructions->command, "export")
			|| !ft_strcmp(bag->mid_bag->instructions->command, "echo")
			|| !ft_strcmp(bag->mid_bag->instructions->command, "unset")
			|| !ft_strcmp(bag->mid_bag->instructions->command, "cd"))
				check_command(bag->mid_bag, var);
			if (!ft_strcmp(bag->mid_bag->instructions->command, "env") || !ft_strcmp(bag->mid_bag->instructions->command, "pwd"))
			{
				i = 1;
				check_command(bag->mid_bag, var);
				if (bag->mid_bag->instructions->out_redirect_type)
				{
					close(fd_2[1]);
					close(STDOUT_FILENO);
					memset(bag->mid_bag->instructions->output_arg, 0, ft_strlen(bag->mid_bag->instructions->output_arg));
					while(i > 0)
					{
						i = read(fd_2[0], pipe_fd, 10);
						pipe_fd[i] = '\0';
						if (i > 0)
						{
							str = ft_strjoin(bag->mid_bag->instructions->output_arg, pipe_fd);
							free(bag->mid_bag->instructions->output_arg);
							bag->mid_bag->instructions->output_arg = ft_strdup(str);
							free(str);
						}
					}
					close(fd_2[0]);
					bag->mid_bag->instructions->echo_option = 1;
					ft_echo(bag->mid_bag->instructions, var);
				}
				else
				{
					close(fd_2[1]);
					close(fd_2[0]);
				}
			}
			else
			{
				close(fd_2[1]);
				close(fd_2[0]);	
			}
			close(fd[1]);
			close(fd[0]);
			if (!builtin_yes_not)
				execve(ft_strjoin("/bin/", bag->mid_bag->instructions->command), bag->mid_bag->instructions->arr_execve, var->var_execve);
			exit(0);
		}
		else
		{
			close(fd_2[1]);
			waitpid(pid_1, &pid_1_ex_status, 0);
			if (!bag->mid_bag->instructions->out_redirect_type)
				dup2(fd_2[0], fd[0]);
			if (bag->mid_bag->instructions->in_redirect_type)
				close(fd_[1]);
			if (bag->mid_bag->instructions->out_redirect_type && !builtin_yes_not)
			{
				i = 1;
				memset(bag->mid_bag->instructions->output_arg, 0, ft_strlen(bag->mid_bag->instructions->output_arg));
				while(i > 0)
				{
					i = read(fd_2[0], pipe_fd, 1000);
					pipe_fd[i] = '\0';
					if (i > 0)
					{
						str = ft_strjoin(bag->mid_bag->instructions->output_arg, pipe_fd);
						free(bag->mid_bag->instructions->output_arg);
						bag->mid_bag->instructions->output_arg = ft_strdup(str);
						free(str);
					}
				}
				close(fd_2[0]);
				bag->mid_bag->instructions->echo_option = 1;
				ft_echo(bag->mid_bag->instructions, var);
			}
			else
				close(fd_2[0]);
			count++;
		}
	}
	free(pipe_fd);
	pipe_fd = NULL;
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



