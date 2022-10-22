NAME = minishell

CC = gcc

CFLAGS = -Werror -Wall -Wextra

RM = rm -rf

SOURCES =	minishell\
			parser\
			utils\
			redirections_utils\
			redirections_2\
			redirections\
			built_in\
			var_handler\
			fork\
			fork_utils\
			initialize\
			free\
			built_in_utils\
			new_split\
			built_in_2\
			executor_utils\
			executor_utils_2\
			executor\
			parser_utils\
			signal\
			var_utils\
			var_utils_2\
			redirections_utils_2\
			f_arg_utils\
			f_arg\
			parser_2\
			minishell_utils\
			minishell_utils_2\
			minishell_utils_3

DEPENDENCIES = libft/libft.a

DIR = ./

SRCS = $(addprefix $(DIR), $(addsuffix .c, $(SOURCES)))

OBJS = $(addprefix $(DIR), $(addsuffix .o, $(SOURCES)))


%.o: %.c 
	@$(CC) $(CFLAGS) -c -o $@ $<

$(NAME): $(OBJS)
	@make -C ./libft
	@$(CC) $(CFLAGS) $(OBJS) -o $@ $(DEPENDENCIES) -lreadline -g

all: $(NAME)

clean:
	@${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

clear_everything: fclean
	@make fclean -C libft
	
re: fclean all

re_everything: clear_everything all

.PHONY: all clean fclean re clear_everything re_everything