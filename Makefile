# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/04 16:42:27 by ylabrahm          #+#    #+#              #
#    Updated: 2023/06/09 13:16:40 by bel-kdio         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

PARS_NAME = minishell_parsing

COMM_SRCS = ./src/common/main.c

PARS_SRCS = ./src/parsing/get_first_command.c ./src/parsing/error.c \
			./src/parsing/colorize.c ./src/parsing/free_double.c \
			./src/parsing/pre_t.c ./src/parsing/remove_quotes.c \
			./src/parsing/set_env.c ./src/parsing/fill_commands.c \
			./src/parsing/lexer.c ./src/parsing/valid_arguments.c \
			./src/parsing/print_error.c ./src/parsing/valid_commands.c\
			./src/parsing/read_heredoc.c\
			./src/exec/exec.c ./src/exec/conv_linked_list.c ./src/builtin/check_if_buil.c\
			./src/builtin/exec_built.c ./src/builtin/exec_cd.c ./src/builtin/built_utils.c \
			./src/builtin/exec_exit.c ./src/builtin/exec_echo.c ./src/builtin/exec_env.c\
			./src/builtin/exec_pwd.c ./src/builtin/exec_unset.c ./src/builtin/exec_export.c\
			./src/exec/set_paths.c ./src/exec/conv_linked_to_tr_p.c ./src/exec/utils_exec.c\

PARS_OBJS = $(PARS_SRCS:.c=.o)

COMM_OBJS = $(COMM_SRCS:.c=.o)

CC = cc

RM = rm -f

# CFLAGS = -Wall -Wextra -Werror
# -fsanitize=address

all:	$(NAME)

$(NAME):	$(COMM_OBJS) $(PARS_OBJS)
	@make -C ./inc/libft/
	$(CC) $(CFLAGS) ./inc/libft/libft.a -fsanitize=address $(COMM_OBJS) $(PARS_OBJS) -lreadline -o $(NAME)

pars:		$(PARS_OBJS)
	@make -C ./inc/libft/
	$(CC) $(CFLAGS) ./inc/libft/libft.a $(PARS_OBJS) -lreadline -o $(PARS_NAME)

clean:
	@make clean -C ./inc/libft/
	$(RM) $(PARS_OBJS)
	$(RM) $(COMM_OBJS)

fclean:	clean
	@make fclean -C ./inc/libft/
	$(RM) $(NAME)
	$(RM) $(PARS_NAME)

re:	fclean all
