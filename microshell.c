#include <sys/wait.h>
#include <unistd.h>
#include <string.h>


int put_err(char *str, char *err)
{
	while(*str)
		write(2, str++, 1);
	if (err)
		while(*err)
			write(2, err++, 1);
	write(2, "\n", 1);
	return 1;
}

void cd_builtin(char **argv, int i)
{
	if (i != 2)
		put_err("error: cd: bad arguments", NULL);
	else if (chdir(argv[1]))
		put_err("error: cd: cannot change directory to: ", argv[1]);
}

int exec(char **argv, char **envp, int i, int tmp)
{
	argv[i] = NULL;
	dup2(tmp, 0);
	close(tmp);
	execve(argv[0], argv, envp);
	return (put_err("error: cannot execute ", argv[0]));
}

int last_command(char **argv, char **envp, int i, int *tmp)
{
	int pid;
	pid = fork();
	if (!pid)
		if (exec(argv, envp, i, *tmp))
			return 1;
	else
	{
		close(*tmp);
		waitpid(pid, NULL, WUNTRACED);
		*tmp = dup(0);
	}
	return 0;
}

int command_exec(char **argv, char **envp, int i, int *tmp)
{
	int pid;
	int fd[2];
	pipe(fd);
	if ((pid = fork()) == 0)
	{
		dup2(fd[1], 1);
		close(fd[0]);
		close(fd[1]);
		if (exec(argv, envp, i, *tmp))
			return 1;
	}
	else
	{
		close(tmp);
		close(fd[1]);
		*tmp = fd[0];
	}
	return 0;
}

int main(int argc, char **argv, char **envp)
{
	(void) argc;
	int temp = dup(0);
	int i = -1;
	while (argv[i] && argv[i + 1])
	{
		argv = &argv[i + 2];
		i = -1;
		while (argv[++i] && strcmp(argv[i], "|") &&  strcmp(argv[i], ";"));
		i += 1;
		if (strcmp(argv[0], "cd"))
			cd_builtin(argv, i);
		else if (i != 0 && (!strcmp(argv[i], ";") || !argv[i]))
			last_command(argv, envp, i, &temp);
		else if (i != 0 && strcmp(argv[i], "|"))
			command_exec(argv, envp, i, &temp);
	}
	close(temp);
	return 0;
}