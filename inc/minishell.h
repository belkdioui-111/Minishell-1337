/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 10:43:06 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/06/22 17:32:15 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"
# include <sys/wait.h>
# include <sys/stat.h>
# include <dirent.h>
# include <errno.h>
# include <string.h>
# include <signal.h>

typedef struct s_env
{
	char			*index;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_globals
{
	int		exit_status;
	int		in_herdoc;
	t_env	*export;
	t_env	*env;
	int		dup;
	int		is_running;
	int		is_interupted;
}t_globals;

extern t_globals	g_glob;

enum e_token_type
{
	TYPE_ARG = 1,
	TYPE_RED_IN = 2,
	TYPE_RED_OUT = 3,
	TYPE_RED_APP = 4,
	TYPE_RED_HER = 5,
	TYPE_RED_PIP = 6,
};

typedef struct s_token_list
{
	char	**tokens;
	int		count;
}	t_token_list;

typedef struct s_check_arg
{
	int		i;
	char	*stack;
	int		top;
	int		s_quote;
	int		d_quote;
	char	*content;
}	t_check_arg;

typedef struct s_sub
{
	char				**sub;
	enum e_token_type	type;
}	t_sub;

typedef struct s_pre_tokens
{
	char				*content;
	enum e_token_type	type;
	int					contain_quotes;
	t_sub				sub;
	struct s_pre_tokens	*next;
	struct s_pre_tokens	*prev;
}	t_pre_tokens;

typedef struct tokenizer_s
{
	int				in_double_quotes;
	char			*us_in;
	int				in_quotes;
	t_pre_tokens	*head;
	int				start;
	int				end;
}	t_tokenizer_t;

typedef struct s_user_data
{
	char	**spl_inp;
	char	*user_input;
}	t_user_data;

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
	int					pipe_hd;
	struct s_command	*next;
	int					has_error;
	int					in_error;
}	t_command;

void			ft_error(char *message);
void			free_double(char **array);
int				add_pre_t(t_pre_tokens **head, char *content, int state);
void			free_linked(t_pre_tokens **head);
void			ft_remove_quotes(t_pre_tokens **head, t_env *head_env);
t_env			*ft_set_env(char **env);
t_command		*ft_fill_commands(t_pre_tokens **head);
void			printf_linked(t_pre_tokens *head);
void			ft_lexer(t_command **head);
t_pre_tokens	*ft_tokenizer(char *user_input);
int				ft_tokenizer_loop(t_tokenizer_t *tok);
void			*ft_init_zeros(t_tokenizer_t *tok);
void			free_commands(t_command **head);
int				add_symbol(t_pre_tokens **head, char *user_input,
					int start, int *end);
int				is_symbol(char symbol);
int				sub_and_add(char *user_input, int start, int end,
					t_pre_tokens **head);
void			printf_env(t_env *head);
void			printf_commands(t_command *head);
char			*ft_read_input(void);
int				sb_ad_ad(char *us_in, int start, int end, t_pre_tokens **head);
int				is_symbol(char symbol);
int				ad_sbl(t_pre_tokens **head, char *us_in, int start, int *end);
void			*ft_init_zeros(t_tokenizer_t *tok);
void			free_linked(t_pre_tokens **head);
void			free_commands(t_command **head);
void			reset_here(char **herdoc);
void			print_leaks(void);
t_command		*get_first_command(char *user_input, t_env *env_head);
int				valid_arguments(t_pre_tokens **head_args);
int				valid_commands(t_command **head_commands, t_env *env_head);
void			print_error(char *error_msg, int error_num);
int				add_pre_t_2(t_pre_tokens **head, char *content,
					t_pre_tokens *node, enum e_token_type type);
int				ft_read_heredoc(t_command **command_ix, t_env *_head);
int				contains_quotes(char *content);
char			*expand_variable(char *token, t_env *head_env, int state);
t_sub			expand_variable_2(t_pre_tokens *node_ix, t_env *head_env);
void			set_node_type(t_pre_tokens **head, int contain_quotes);
char			**expand_redirs_2(char *var, t_env *env);
int				is_valid_variable(char after_dollar);
void			four_free(char **tk, char **sf, char **evix, char **evvl);
char			*get_new_token(char **token, char *new, t_env *head_env, int i);
void			expand_loop_1(char **token, int in_single_quote,
					int in_double_quote, int i);
void			expand_loop_2(char **token, int in_single_quote,
					int in_double_quote, int i);
int				set_up_remove_vars(int *j, int *in_single, int *in_double);
t_sub			get_sub_from_node(t_pre_tokens **node_ix);

//execution part start

//expanding
char			*exit_st(char *key);
char			*expand_redirs(char *var);
char			*expand_red(t_pre_tokens *node, int *ambiguous);
int				get_len(char *var);
char			*expnd(char *var, int *amb);
char			*get_index(char *idx);
char			*get_value(char *idx, t_env **env);
//end expanding

//redirections start
int				redirection(t_command *head);
int				perr(char *arg, char *msg, int ret);
int				check_if_output(int *cmdn, char *cmd, t_pre_tokens *out);
int				check_if_input(int *cmdn, t_command *head, t_pre_tokens *in);
int				check_if_heredocs(t_command *head);
// redirections end

char			**convert_link_to_2p(void);
int				calculate_len_of_w(t_command *all_cmd, int i);
int				calculate_number_of_args_in_node(t_command *all_cmd);
int				calculate_num_of_cmd(t_command *all_cmd);
void			conver_l_args_to_p(t_command *head_command);
void			exec(char ***all_cmd, t_command *head);
void			simple_execute(char **cmd, int *pipes, int fd, t_command *node);
char			***convert_linked_list_to_tr_p(t_command *head_command);
int				pr_err(char *str1, char *str2, char *str3, int status);
void			free_all_cmd(char ***all_cmd);
void			check_paths(char *path, char *cmd);

//set paths start
char			*set_path(t_command *head_command);
char			*remove_quote(char *content);
//set paths end

//built part start
void			if_there_is_index_and_value(char *args);
char			*search_in_env_and_return_value(t_env *env, char *s);
int				loop_for_equal(char *args);
int				check_syntax_export(char *args);
void			mod_env_exp(t_command *cmd);
int				exec_export(t_command *cmds);
int				exec_unset(char **args);
int				exec_pwd(void);
int				exec_env(void);
int				exec_echo(t_command *cmds);
int				exec_cd(t_command *cmd);
int				exec_exit(t_pre_tokens *args, int status);
void			search_in_env_and_replace(t_env *env, char *index, char *str);
int				search_in_env(t_env *env, char *s);
int				check_if_buil(char *s);
int				exec_built(int n, t_command *cmds);
//built part end

//execution part end

#endif