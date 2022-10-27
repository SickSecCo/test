#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int err(char *a, char *b)
{
    while (*a)
        write(2, a++, 1);
    if (b)
        while(*b)
            write(2, b++, 1);
    write(2, "\n", 1);
    return (1);
}

int exec(char **argv, int i, int tmp, char **envp)
{
    argv[i] = NULL;
    dup2(tmp, 0);
    close(tmp);
    execve(argv[0], argv, envp);
    return (err("error: cannot execute ", argv[0]));
}

int main(int argc, char **argv, char **envp)
{
    int i = 0;
    int tmp = dup(0);
    int pid = 0;
    int fd[2];
    (void)argc;
    while (argv[i] && argv[i + 1])
    {
        argv = &argv[i + 1];
        i = 0;
        while (argv[i] && strcmp(argv[i], ";") && strcmp(argv[i], "|"))
            i++;
        if (!strcmp(argv[0], "cd"))
        {
            if (i != 2)
                err("error: cd: bad arguments", NULL);
            else if (chdir(argv[1]) != 0)
                err("error: cd: cannot change directory to: ", argv[1]);
        }
        else if (i != 0 && (argv[i] == NULL || !strcmp(argv[i], ";")))
        {
            pid = fork();
            if (!pid)
            {
                if (exec(argv, i, tmp, envp))
                    return (1);
            }
            else
            {
                close(tmp);
                while (waitpid(-1, NULL, WUNTRACED) != -1)
                    ;
                tmp = dup(0);
            }
        }
        else if (i != 0 && !strcmp(argv[i], "|"))
        {
            pipe(fd);
            pid = fork();
            if (!pid)
            {
                dup2(fd[1], 1);
                close(fd[0]);
                close(fd[1]);
                if (exec(argv, i, tmp, envp))
                    return (1);
            }
            else
            {
                close(tmp);
                close(fd[1]);
                tmp = fd[0];
            }
        }
    }
    close(tmp);
    return (0);
}