#include "minishell.h"

int	complete_arr_execve(t_commands **instructions, char *s)
{
	int	k;

	k = 0;
	(*instructions)->arr_execve[k++] = ft_strdup((*instructions)->command);
	(*instructions)->arr_execve = ft_splitto((*instructions)->arr_execve, (*instructions)->output_arg, 32, &k);
	(*instructions)->arr_execve_index = k;
	(*instructions)->arr_execve[(*instructions)->arr_execve_index] = NULL;
}


int	fork_single_command(int link[], t_bag *bag, t_var *var)
{
	if (bag->mid_bag->instructions->out_redirect_type)
		dup2(link[1], STDOUT_FILENO);
	close(link[0]);
	close(link[1]);
	if (!ft_strcmp(bag->mid_bag->instructions->command, "pwd") || !ft_strcmp(bag->mid_bag->instructions->command, "env"))
	{
		check_command(bag, var);
		return (0);
	}
	else
		execute_command(bag, var);
	return(127);
}

void fd_backup_(int fd[], int i)
{
	if (i == 0)
	{
		fd[0] = dup(STDIN_FILENO); 
		fd[1] = dup(STDOUT_FILENO);
	}
	else
	{
		dup2(fd[0], STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
	} 
}

void	set_signal(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	set_env_error(t_bag *bag, int wstatus)
{
	if (env_error != 130 && env_error != 131)
		env_error = WEXITSTATUS(wstatus);
	bag->err_env_err = 1;
}

int    ft_fork(t_bag *bag, t_var *var)
{
    int pid;
    int wstatus;
    int link[2];
	int fd;
	int fd_backup[2];

	fd_backup_(fd_backup, 0);
	if (!ft_strcmp_mini(bag->mid_bag->instructions->command, "minishell"))
		set_signal();
	if (bag->mid_bag->instructions->in_redirect_type || bag->mid_bag->instructions->heredocs_switch)
		in_redirect_fork_util(bag, fd);
    pipe(link);
    pid = fork();
    if (pid == 0)
		exit (fork_single_command(link, bag, var));
	close(link[1]);
	read_util(bag, link);
	close(link[0]);
	waitpid(pid, &wstatus, 0);
	set_env_error(bag, wstatus);	
	fd_backup_(fd_backup, 1);
}

int	fork_loop_second_child(t_bag *bag, int fd[], int fd_2[], t_var *var)
{
	int already_closed;
	
	already_closed = 0;
	if (is_built_in_check(bag, 0))
		check_command(bag, var);
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
	return(127);
}

int	fork_loop_first_child(t_bag *bag, int fd[], int fd_, t_var *var)
{
	int already_closed;

	already_closed = 0;
	if (bag->mid_bag->instructions->in_redirect_type || bag->mid_bag->instructions->heredocs_switch)
		in_redirect_fork_util(bag, fd_);
	dup2(fd[1], STDOUT_FILENO);
	if (is_built_in_check(bag, 0))
		check_command(bag, var);
	if (!ft_strcmp(bag->mid_bag->instructions->command, "env") || !ft_strcmp(bag->mid_bag->instructions->command, "pwd"))
		read_fork_pipe(bag, fd, &already_closed, var);
	if (!already_closed)
	{
		close(fd[1]);
		close(fd[0]);
	}
	if (!bag->mid_bag->instructions->builtin_yes_not)
		execute_command(bag, var);
	return(127);
}
