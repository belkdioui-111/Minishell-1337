/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 10:43:06 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/06/01 17:14:14 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"
#include <wait.h>

enum token_type
{
	TYPE_ARG,
	TYPE_RED_IN,
	TYPE_RED_OUT,
	TYPE_RED_APP,
	TYPE_RED_HER,
	TYPE_RED_PIP,
};

typedef struct s_env
{
	char			*index;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_check_arg
{
	int		i;
	char	*stack;
	int		top;
	int		s_quote;
	int		d_quote;
	char	*content;
}	t_check_arg;

typedef struct s_pre_tokens
{
	char				*content;
	enum token_type		type;
	struct s_pre_tokens	*next;
	struct s_pre_tokens	*prev;
}	t_pre_tokens;

typedef struct tokenizer_s
{
	int 			in_double_quotes;
	char			*user_input;
	int 			in_quotes;
	t_pre_tokens 	*head;
	int 			start;
	int 			end;
}	tokenizer_t;

typedef struct s_user_data
{
	char	**spl_inp;
	char	*user_input;
}   t_user_data;


// - - - - - - - - - - - - - - 
// typedef struct s_arguments
// {
// 	char				*content;
// 	struct s_arguments	*next;
// }	t_arguments;

// - - - - - - - - - - - - - - 
typedef struct s_command
{
	char				*cmd;
	t_pre_tokens		*args;
	t_pre_tokens		*output_files;
	t_pre_tokens		*input_files;
	t_pre_tokens		*append_files;
	t_pre_tokens		*herdoc_files;
	char				**db_args;
	char				*path;
	struct s_command	*next;
}	t_command;

// ls -la arg1 arg2 > out > in > here g > hey > heeey

void			ft_error(char *message);
void			free_double(char **array);
char			*ft_colorize(char *message, char *color);
int				add_pre_t(t_pre_tokens **head, char *content);
void			free_linked(t_pre_tokens **head);
void			ft_remove_quotes(t_pre_tokens **head, t_env *head_env);
t_env			*ft_set_env(char **env);
t_command		*ft_fill_commands(t_pre_tokens **head);
void			printf_linked(t_pre_tokens *head);
void			ft_lexer(t_command **head);
t_pre_tokens	*ft_tokenizer(char *user_input);
int				ft_tokenizer_loop(tokenizer_t *tok);
void 			*ft_init_zeros(tokenizer_t *tok);
void			free_commands(t_command **head, char ***cmd);
void			free_linked(t_pre_tokens **head);
int				add_symbol(t_pre_tokens **head, char *user_input, int start, int *end);
int				is_symbol(char symbol);
int				sub_and_add(char *user_input, int start, int end, t_pre_tokens **head);
void			printf_env(t_env *head);
void			printf_commands(t_command *head);
void			printf_linked(t_pre_tokens *head);
char			*ft_read_input();
int 			ft_cnt(char *string);
void			print_leaks();
t_command		*get_first_command(char *user_input, t_env *env_head);
int				valid_arguments(t_pre_tokens **head_args);
int				valid_commands(t_command **head_commands);
void			print_error(char *error_msg);

void    conver_l_args_to_p(t_command *head_command);

char    ***convert_linked_list_to_tr_p(t_command *all_cmd);
void    exec(char ***all_cmd, t_command *head, char **envp);
void 	set_path(t_command *head_command, char  **env);
int    check_if_buil(char *s, t_command *cmds);
int    exec_built(int n, t_command *cmds, t_env *env, t_env *export_head);


/*
	cases :
		 "> out echo "hello" > in"
*/