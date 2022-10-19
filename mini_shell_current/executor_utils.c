#include "minishell.h"

int	in_redirect_fork_util(t_bag *bag, int fd_)
{
	if (bag->mid_bag->instructions->file_input || bag->mid_bag->instructions->in_redirect_type != 2)
	{
		fd_ = open(bag->mid_bag->instructions->file_input, O_RDONLY, 0666);
		if (fd_ > 0)
		{
			dup2(fd_, STDIN_FILENO);
			close(fd_);
		}
		else
		{
			printf("%s: No such file or directory\n", bag->mid_bag->instructions->file_input);
			return (0);
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
	check_command(bag, var);
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
