#include "minishell.h"

int	complete_arr_execve(t_commands **instructions, int i)
{
	int		j = 0;
	int		k, h;

	i += 1;
	k = 0;
	h = 0;
	(*instructions)->arr_execve[k++] = ft_strdup((*instructions)->command);
	(*instructions)->arr_execve = ft_splitto((*instructions)->arr_execve, (*instructions)->output_arg, 32, &k);
	(*instructions)->arr_execve_index = k;
	//DEBUG
	// int jk = 0;
	// while (jk < (*instructions)->arr_execve_index)
	// {
	// 	printf("---comando: %s---\n", (*instructions)->arr_execve[jk]);
	// 	jk++;
	// }
	// printf("\n");
	
}


















int    ft_fork(t_bag *bag, t_var *var)
{
    int pid;
    int wstatus;
    int link[2];
	int fd[2];
    int i;
	int p;
    char *pipe_fd;
	int	k;

    i = 1;
	pipe_fd = malloc(sizeof(char) * 10001);
	// ft_var(var);
	if (!ft_strcmp_mini(bag->mid_bag->instructions->command, "minishell"))
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
	else
		complete_arr_execve(&bag->mid_bag->instructions, -1);
	if (bag->mid_bag->instructions->in_redirect_type || bag->mid_bag->instructions->heredocs_switch)
	{
		if (bag->mid_bag->instructions->file_input || bag->mid_bag->instructions->in_redirect_type != 2)
		{
			fd[0] = open(bag->mid_bag->instructions->file_input, O_RDONLY, 0666);
			if (fd[0] > 0)
			{
				fd[1] = dup(STDIN_FILENO);
				dup2(fd[0], STDIN_FILENO);
				close(fd[0]);
			}
			else 
			{
				printf("%s: No such file or directory\n", bag->mid_bag->instructions->file_input);
				return 0;
			}
		}
		else if (bag->mid_bag->instructions->in_redirect_type == 2)
		{
			bag->mid_bag->instructions->heredoc_fd = open("here_doc.txt", O_RDONLY, 0666);
			if (bag->mid_bag->instructions->heredoc_fd)
			{
				fd[1] = dup(STDIN_FILENO);
				dup2(bag->mid_bag->instructions->heredoc_fd, STDIN_FILENO);
				close(bag->mid_bag->instructions->heredoc_fd);
			}
		}
	}
    pipe(link);
    pid = fork();
    if (pid == 0)
    {
		if (bag->mid_bag->instructions->out_redirect_type)
			dup2(link[1], STDOUT_FILENO);
		close(link[0]);
        close(link[1]);
		if (!ft_strcmp(bag->mid_bag->instructions->command, "pwd") || !ft_strcmp(bag->mid_bag->instructions->command, "env"))
		{
            check_command(bag->mid_bag, var);
            exit(0);
        }
        else
		{
        	p = execve(ft_strjoin("/bin/", bag->mid_bag->instructions->command), bag->mid_bag->instructions->arr_execve, var->var_execve);
			if (p != 0)
			{
				k = 0;
				while (k < var->count)
				{
					if (!ft_strcmp(var->var_name[k], "PWD"))
						break ;
					k++;
				}
				p = execve(ft_strjoin(ft_strjoin(var->var_value[k], "/"), bag->mid_bag->instructions->command), bag->mid_bag->instructions->arr_execve, var->var_execve);
			}
			if (p != 0)
				execve(bag->mid_bag->instructions->command, bag->mid_bag->instructions->arr_execve, var->var_execve);
		}
		printf("%s: command not found\n", bag->mid_bag->instructions->command);
		exit(127);
    }
	else
    {
        close(link[1]);
        memset(bag->mid_bag->instructions->output_arg, 0, ft_strlen(bag->mid_bag->instructions->output_arg));
        char *str;
        while(i > 0)
        {
            i = read(link[0], pipe_fd, 10000);
			pipe_fd[i] = '\0';
			if (i > 0)
			{
				if (bag->mid_bag->instructions->in_redirect_type || bag->mid_bag->instructions->out_redirect_type)
				{
					str = ft_strjoin(bag->mid_bag->instructions->output_arg, pipe_fd);
					free(bag->mid_bag->instructions->output_arg);
					bag->mid_bag->instructions->output_arg = ft_strdup(str);
					free(str);
				}
			}
        }
		close(link[0]);
		waitpid(pid, &wstatus, 0);
		if (bag->mid_bag->instructions->in_redirect_type)
			dup2(fd[1], STDIN_FILENO);
		if (env_error != 130 && env_error != 131)
			env_error = WEXITSTATUS(wstatus);
		bag->mid_bag->err_env_err = 1;
		free(pipe_fd);
		return (1);
    }
}
