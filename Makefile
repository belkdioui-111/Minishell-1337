# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/04 16:42:27 by ylabrahm          #+#    #+#              #
#    Updated: 2023/06/11 10:08:44 by bel-kdio         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

LIBFT = ./inc/libft/libft.a

SRC =./src/common/main.c ./src/parsing/get_first_command.c ./src/parsing/error.c \
	./src/parsing/colorize.c ./src/parsing/free_double.c \
	./src/parsing/pre_t.c ./src/parsing/remove_quotes.c \
	./src/parsing/set_env.c ./src/parsing/fill_commands.c \
	./src/parsing/lexer.c ./src/parsing/valid_arguments.c \
	./src/parsing/print_error.c ./src/parsing/valid_commands.c \
	./src/parsing/read_heredoc.c \
	./src/exec/exec.c ./src/exec/conv_linked_list.c ./src/builtin/check_if_buil.c \
	./src/builtin/exec_built.c ./src/builtin/exec_cd.c ./src/builtin/built_utils.c \
	./src/builtin/exec_exit.c ./src/builtin/exec_echo.c ./src/builtin/exec_env.c \
	./src/builtin/exec_pwd.c ./src/builtin/exec_unset.c ./src/builtin/exec_export.c \
	./src/exec/set_paths.c ./src/exec/conv_linked_to_tr_p.c ./src/exec/utils_exec.c \
	./src/builtin/exec_export_mod.c ./src/exec/calculating.c ./src/exec/redirections.c \

OBJ = $(SRC:.c=.o)

CC = cc

RM = rm -f

CFLAGS =  #-fsanitize=address #-Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJ)
	@make -C ./inc/libft/
	$(CC) $(OBJ) $(LIBFT) $(CFLAGS) -lreadline -o $(NAME)

clean:
	@make clean -C ./inc/libft/
	$(RM) $(OBJ)

fclean:	clean
	@make fclean -C ./inc/libft/
	$(RM) $(NAME)

re:	fclean all
