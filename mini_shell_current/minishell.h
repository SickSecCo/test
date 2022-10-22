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
	int		inputs_count;
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
}	t_commands;

typedef struct	s_tree
{
	unsigned int		priority_id;
	unsigned int		words_count;
	int					pipe_yes_not;
	char				*raw_command;
	struct s_tree		*next;
	struct s_tree		*prev;
	struct s_commands	*instructions;
}	t_tree;
  
typedef struct s_bag
{
	int				commands_count;
	struct s_tree	*mid_bag;
	int				err_env_err;
}	t_bag;

typedef struct s_fork
{
	int pid;
	int pid_1;
	int pid_ex_status;
	int pid_1_ex_status;
	int count;
	int	commands_count;
}	t_fork;

typedef struct s_index
{
	int	i;
	int	j;
	int	count;
	int	type;
}	t_index;


typedef struct s_handler
{
	int 	*index;
	char	*dest;
}	t_handler;

typedef struct s_fd
{
	int	fd[2];
	int	fd_2[2];
	int	fd_backup[2];
}	t_fd;

extern int g_env_error;

int		take_delimiters(t_commands **instructions, int i);

int		ft_strcmp(char *s1, char *s2);

void	take_inputs_loop(t_commands *instructions);

int		parse_string_commands(t_tree *bag, int commands_count, t_var *var);

int		check_quotes(t_commands *instruction);

void	ft_check_redi(t_commands *instructions);

int		ft_min_single_file(t_commands **instructions, int i, char *str);

int		multiple_file(char *input);

char	*f_arg(char *input, t_commands *instruction);

void	signal_handler(int code, siginfo_t *siginfo, void *k);

int		parse_string_pipe(t_bag *bag, char *input);

void	print(t_tree *bag, int commands_count); //DEBUG

void    ft_pwd();

void	check_command(t_bag *bag, t_var *var);

void    ft_cd(t_bag *mid_bag, t_var *var);

void	ft_echo(t_commands *instructions, t_var *var);

void	ft_env(t_var *var);

int		check_argument_export(t_var *var, t_tree *bag);

void	ft_export(t_var *var, t_bag *bag);

void	ft_unset(t_var *var, t_bag *bag);

void	ft_var_change(t_var *var, int i);

void	check_arguments_unset(t_var *var, t_tree *bag);

char	*ft_var_out(char *str, t_commands *instruction, t_var *var, int i);

int		check_inputs(t_var *var, char *input);

int		parse(t_bag *bag, char *input, t_var *var);

int	ft_fork(t_bag *bag, t_var *var);

void	set_termios(int code);

int		figure_count(void);

int     ft_strcmp_mini(char *str1, char *str2);

void	ft_var(t_var *var);

void	initialize_var(t_var *var, char **envp);

void    loop_free(t_var *var, t_bag *bag, int commands_count2);

void	complete_arr_execve(t_commands **instructions);

void	signal_handler_1(int code, siginfo_t *siginfo, void *k);

t_tree	*initialize_instructions(t_tree *bag);

int		check_argument_export_exit(t_tree *bag, char *string, int i);

void	free_no_input(t_bag *bag, t_var *var, char *s, char *prompt);

int		main_loop_core(t_bag *bag, t_var *var, char *s);

void	ft_putnbr(char **str, int j, int k, int i);

void	signal_handler_4(int code, siginfo_t *siginfo, void *k);

char	**ft_splitto(char **array, char *s, char c, int *starter);

int		is_built_in_check(t_bag *bag, int code);

int		fork_loop_first_child(t_bag *bag, int fd[], t_var *var);

void	is_built_in_set(t_bag *bag, int code);

int		charsep(char c, char del);

int		count_chars(char *str, char del);

void	write_oneword(char *dst, char *src, char del);

int		strsplitwrite(char **str2, char *str, char del, int starter);

int		execute_command(t_bag *bag, t_var *var);

void	read_util(t_bag *bag, int fd[]);

int	in_redirect_fork_util(t_commands *instructions);

int		fork_loop_second_child(t_bag *bag, int fd[], int fd_2[], t_var *var);

void	read_fork_pipe(t_bag *bag, int fd[], int *already_closed, t_var *var);

char	**split_basic(char *s, char c, int *starter);

void	ft_cd_util(t_var *var);

void	set_signal(void);

void	set_signal(void);

void	set_env_error(t_bag *bag, int wstatus);

void	is_built_in_set(t_bag *bag, int code);

int		is_built_in_check(t_bag *bag, int code);

int		fork_single_command(int link[], t_bag *bag, t_var *var);

void	fd_backup_(int fd[], int i);

void	ft_advance(t_handler ** handle, char *str, t_commands *instruction);

void	init_handle(t_handler **handle, t_commands *instruction);

void	ft_int_handle(char *dest, int *index);

void	ft_no_name(char *dest, char *str, int *index);

void    ft_name(char *dest, t_var *var, int *index, int *count);

void	ft_go(char *str, int *index);

void    ft_var_while(t_handler **handle, char *str, t_var *var, int *count);

struct sigaction	set_signal_quit_ignore(void);

void	take_inputs_loop(t_commands *instructions);

void	sig_default(void);

void	ft_second(t_commands *instruction, char *input, char **temp, int **index);

void	ft_first(char *input, int **index);

void	ft_third(t_commands *instruction, char *input, char **temp, int **index);

void	ft_cpy(char **temp, int **index, char *input);

void	take_inputs_loop_util(t_commands *instructions);

void	ft_mix(t_commands *instruction, char *input, char **temp, int **index);

void	ft_maj_if(t_commands *instructions, char *file, int **index, char *str);

void	ft_min_if(t_commands *instructions, int **index, char *str);

void	min_single_file_2(t_commands **instructions, int i, int j, int fd);

void	parse_pipe_2(char *input, t_bag *bag, size_t *i, int *j);

int	ft_echo_option(t_tree *bag, int i);

void	ft_arg_split(t_tree *bag, int *i, int *j);

t_index	*ft_index(void);

void	ft_mult_quotes(t_index *index, t_commands *instruction);

t_bag	*bag_initialize(struct sigaction sig);

void	fork_multiple_commands(t_bag *bag, t_var *var, int commands_count);

void	fork_second_parent(t_fd *fd, t_bag *bag, t_var *var, t_fork *f_stats);

#endif