SRC = main.c exec.c
LIBFT = libft/libft.a
OBJ = $(SRC:.c=.o)

CC = cc
CFLAGS = -Wall -Werror -Wextra -fsanitize=address
NAME = exec

all: $(NAME)

$(NAME): $(OBJ)
	@make -C libft/
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)
		
clean :
	rm -rf $(OBJ)
	@make fclean -C libft/
	
fclean : clean
	rm -rf $(NAME)

re: fclean $(NAME)
