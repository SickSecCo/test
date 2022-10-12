#ifndef _MINISHELL_H
# define _MINISHELL_H

# include <readline/history.h>
# include <readline/readline.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sys/time.h>
# include <sys/resource.h>
# include <signal.h>
# include <dirent.h>
# include <string.h>
# include <errno.h>
# include <sys/ioctl.h>
# include <termios.h>
# include <term.h>
# include <curses.h>
# include "libft/libft.h"

typedef struct s_var
{
	char	**var_name;
	char	**var_value;
	char	**var_execve;
	int		count;
	char	*name;
	char	*value;
	char	**array_of_inputs;
	unsigned int	inputs_count;
}	t_var;

typedef struct	s_commands
{
	int				heredocs_switch;
	int				out_redirect_type;
	int				in_redirect_type;
	int				echo_option;
	int				err_yes_no;
	int				arr_file_index;
	int				arr_execve_index;
	int				multiple;
	int				delimiters_count;
	int				heredoc_string_count;
	char			*command;
	char			*argument;
	char 			*output_arg;
	char			*file_output;
	char			*file_input;
	char			**arr_file;
	char			**arr_execve;
	char			**arr_delimiters;
	char			**heredoc_array;
	int				heredoc_fd;
	int				*quote_flag;
	int				*quote_start;
	int				*quote_end;
	int				quote_count;
	int				builtin_yes_not;
	int				fd_backup;
}	t_commands;

typedef struct	s_tree
{
	unsigned int		priority_id;
	unsigned int		words_count;
	int					err_env_err;
	int					pipe_yes_not;
	char				*raw_command;
	struct s_tree		*next;
	struct s_tree		*prev;
	struct s_commands	*instructions;
}	t_tree;

typedef struct s_bag
{
	struct s_tree	*mid_bag;
}	t_bag;

extern int env_error;



void	main_loop(struct sigaction sig, char **envp);

int		take_delimiters(t_commands **instructions, int i);

int		double_min_multiple_files(t_commands **instructions, int i);

int		ft_strcmp(const char *s1, const char *s2);

void	take_inputs_loop(t_commands *instructions);

int		parse_string_commands(t_tree *bag, int commands_count, t_var *var);

int		check_quotes(t_commands *instruction);

void	ft_check_redi(t_commands *instructions);

int		ft_min_single_file(t_commands **instructions, int i);

int		ft_min_multiple_files(t_commands **instructions, int i);

int		multiple_file(char *input);

char	*f_arg(char *input, t_commands *instruction);

void	signal_handler(int code, siginfo_t *siginfo, void *k);

int		parse_string_pipe(t_bag *bag, char *input);

void	print(t_tree *bag, int commands_count); //DEBUG

void    ft_pwd();

void	check_command(t_tree *bag, t_var *var);

void    ft_cd(t_tree *mid_bag, t_var *var);

void	ft_echo(t_commands *instructions, t_var *var);

void	ft_env(t_var *var);

int		check_argument_export(t_var *var, t_tree *bag);

void	ft_export(t_var *var, t_tree *bag);

void	ft_unset(t_var *var, t_tree *bag);

void	ft_var_change(t_var *var, int i);

void	check_arguments_unset(t_var *var, t_tree *bag);

char	*ft_var_output(char *str, t_commands *instruction, t_var *var, int i);

int		check_inputs(t_var *var, char *input);

int		parse(t_bag *bag, char *input, t_var *var);

int		ft_fork(t_bag *bag, t_var *var);

void	set_termios(int code);

int		figure_count(void);

int     ft_strcmp_mini(char *str1, char *str2);

void	ft_var(t_var *var);

void	initialize_var(t_var *var, char **envp);

void    loop_free(t_var *var, t_bag *bag, int commands_count2);

int	complete_arr_execve(t_commands **instructions, int i);

void	signal_handler_1(int code, siginfo_t *siginfo, void *k);

t_tree	*initialize_instructions(t_tree *bag, int commands_count);

int	check_argument_export_exit(t_tree *bag, char *string, int i);

void	free_no_input(t_bag *bag, t_var *var, char *s, char *prompt);

int	main_loop_core(t_bag *bag, t_var *var, char *s);

char	*ft_putnbr(char *str, int j, int k, int i);

void	signal_handler_4(int code, siginfo_t *siginfo, void *k);

char	**ft_splitto(char **array, char const *s, char c, int *starter);

int		is_built_in_check(t_bag *bag, int code);

int		fork_loop_first_child(t_bag *bag, int fd[], int fd_[], t_var *var);

void	is_built_in_set(t_bag *bag, int code);





#endif