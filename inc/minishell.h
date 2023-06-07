/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 10:43:06 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/06/07 18:43:56 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"
#include <sys/wait.h>

#include <sys/stat.h>
#include <dirent.h>


struct GlobalVariables {
    int exit_status;
};

struct GlobalVariables globals;

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
	int					contain_quotes;
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
		int					cmd_type;
		t_pre_tokens		*args;
		t_pre_tokens		*output_files;
		t_pre_tokens		*input_files;
		t_pre_tokens		*append_files;
		t_pre_tokens		*herdoc_files;
		int					out_type;
		int					in_type;
		char				**db_args;
		char				*path;
		char				*here_doc_data;
		struct s_command	*next;
		int					has_error;
}	t_command;

// ls -la arg1 arg2 > out > in > here g > hey > heeey

void			ft_error(char *message);
void			free_double(char **array);
char			*ft_colorize(char *message, char *color);
int				add_pre_t(t_pre_tokens **head, char *content, int state);
void			free_linked(t_pre_tokens **head);
t_pre_tokens	*ft_remove_quotes(t_pre_tokens **head, t_env *head_env);
t_env			*ft_set_env(char **env);
t_command		*ft_fill_commands(t_pre_tokens **head);
void			printf_linked(t_pre_tokens *head);
void			ft_lexer(t_command **head);
t_pre_tokens	*ft_tokenizer(char *user_input);
int				ft_tokenizer_loop(tokenizer_t *tok);
void 			*ft_init_zeros(tokenizer_t *tok);
void			free_commands(t_command **head);
int				add_symbol(t_pre_tokens **head, char *user_input, int start, int *end);
int				is_symbol(char symbol);
int				sub_and_add(char *user_input, int start, int end, t_pre_tokens **head);
void			printf_env(t_env *head);
void			printf_commands(t_command *head);
char			*ft_read_input();
int 			ft_cnt(char *string);
void			print_leaks();
t_command		*get_first_command(char *user_input, t_env *env_head);
int				valid_arguments(t_pre_tokens **head_args);
int				valid_commands(t_command **head_commands);
void			print_error(char *error_msg);
void    conver_l_args_to_p(t_command *head_command);
char    ***convert_linked_list_to_tr_p(t_command *all_cmd);
void	exec(char ***all_cmd, t_command *head, t_env *exp, t_env *env);
void 	set_path(t_command *head_command, t_env *env_head);
int    check_if_buil(char *s, t_command *cmds);
int    exec_built(int n, t_command *cmds, t_env *env, t_env *export_head);
///
int				add_pre_t_2(t_pre_tokens **head, char *content, t_pre_tokens *node);
int				calculate_num_of_cmd(t_command *all_cmd);
int				calculate_number_of_args_in_node(t_command *all_cmd);
int				calculate_len_of_w(t_command *all_cmd, int i);
void			fill(t_command *all_cmd, int i, char *ev_word);
void			fill_it(char **arr_of_ev_cmd, t_command *all_cmd);
void			fill_arr_of_all(char ***arr_of_all_cmd, t_command *all_cmd);
char			***convert_linked_list_to_tr_p(t_command *head_command);
void			ft_read_heredoc(t_command **command_ix);
char			*search_in_env(t_env *env, char *s);

///

/*
	cases :
		 "> out echo "hello" > in"
*/