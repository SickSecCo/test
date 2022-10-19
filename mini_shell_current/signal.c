#include "minishell.h"

void	signal_handler_4(int code, siginfo_t *siginfo, void *k)
{
	exit(0);
}

void	signal_handler(int code, siginfo_t *siginfo, void *k)
{
	if(code == SIGQUIT)
	{
		rl_on_new_line();
		rl_redisplay();
	}
	else if (code == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		env_error = 130;
	}
}

void	signal_handler_1(int code, siginfo_t *siginfo, void *k)
{
	if (code == SIGINT)
		env_error = 130;
	else
		env_error = 131;
	write(1, "\n", 1);
}

void	set_termios(int code)
{
	struct termios			new;
	static struct termios	old;

	if (code == 1)
	{
		tcgetattr(STDIN_FILENO, &old);
		new = old;
		new.c_lflag &= ~(ECHOCTL);
		tcsetattr(STDIN_FILENO, TCSANOW, &new);
	}
	else
		tcsetattr(STDIN_FILENO, TCSANOW, &old);
}
