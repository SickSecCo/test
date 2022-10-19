NAME = minishell

CC = gcc

# CFLAGS = -Werror -Wall -Wextra

RM = rm -rf

SOURCES =	minishell\
			parser\
			utils\
			redirections_utils\
			redirections\
			built_in\
			var_handler\
			fork\
			initialize\
			free\
			built_in_utils\
			new_split\
			built_in_2\
			executor_utils\
			executor\
			parser_utils\
			signal\

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

.PHONY: all clean fclean re clear_e