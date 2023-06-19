/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 10:43:06 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/06/19 18:50:56 by bel-kdio         ###   ########.fr       */
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

typedef struct s_globals	 {
	int	exit_status;
}t_globals;

extern t_globals	glob;

enum token_type
{
	TYPE_ARG = 1,
	TYPE_RED_IN = 2,
	TYPE_RED_OUT = 3,
	TYPE_RED_APP = 4,
	TYPE_RED_HER = 5,
	TYPE_RED_PIP = 6,
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

typedef struct s_sub
{
	char				**sub;
	enum token_type		type;
}	t_sub;

typedef struct s_pre_tokens
{
	char				*content;
	enum token_type		type;
	int					contain_quotes;
	t_sub				sub;
	struct s_pre_tokens	*next;
	struct s_pre_tokens	*prev;
}	t_pre_tokens;

typedef struct tokenizer_s
{
	int				in_double_quotes;
	char			*user_input;
	int				in_quotes;
	t_pre_tokens	*head;
	int				start;
	int				end;
}	tokenizer_t;

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
char			*ft_colorize(char *message, char *color);
int				add_pre_t(t_pre_tokens **head, char *content, int state);
void			free_linked(t_pre_tokens **head);
void			ft_remove_quotes(t_pre_tokens **head, t_env *head_env);
t_env			*ft_set_env(char **env);
t_command		*ft_fill_commands(t_pre_tokens **head);
void			printf_linked(t_pre_tokens *head);
void			ft_lexer(t_command **head);
t_pre_tokens	*ft_tokenizer(char *user_input);
int				ft_tokenizer_loop(tokenizer_t *tok);
void			*ft_init_zeros(tokenizer_t *tok);
void			free_commands(t_command **head);
int				add_symbol(t_pre_tokens **head, char *user_input, int start, int *end);
int				is_symbol(char symbol);
int				sub_and_add(char *user_input, int start, int end, t_pre_tokens **head);
void			printf_env(t_env *head);
void			printf_commands(t_command *head);
char			*ft_read_input();
int				ft_cnt(char *string);
void			print_leaks();
t_command		*get_first_command(char *user_input, t_env *env_head);
int				valid_arguments(t_pre_tokens **head_args);
int				valid_commands(t_command **head_commands, t_env *env_head);
void			print_error(char *error_msg, int error_num);
int				add_pre_t_2(t_pre_tokens **head, char *content, t_pre_tokens *node, enum token_type type);
int				ft_read_heredoc(t_command **command_ix, t_env *env_head);
int				contains_quotes(char *content);
char			*expand_variable(char *token, t_env *head_env, int state);
t_sub			expand_variable_2(t_pre_tokens **node_ix, t_env *head_env);
void			set_node_type(t_pre_tokens **head, int contain_quotes);





//execution part start
char	*expand_red(t_pre_tokens *node, int *ambiguous, t_env *env_head);

int				redirection(t_command *head, t_env *env);
char			**convert_link_to_2p(t_env *env);
int				calculate_len_of_w(t_command *all_cmd, int i);
int				calculate_number_of_args_in_node(t_command *all_cmd);
int				calculate_num_of_cmd(t_command *all_cmd);
void			conver_l_args_to_p(t_command *head_command);
void			exec(char ***all_cmd, t_command *head, t_env *exp, t_env *env);
char			***convert_linked_list_to_tr_p(t_command *head_command);
int				pr_err(char *str1, char *str2, char *str3, int status);
//set paths start
char			*set_path(t_command *head_command, t_env *env_head);
char			*remove_quote(char *content);

//set paths end

//built part start
void			mod_env_exp(t_env *env, t_env *exp, t_command *cmd);
int				exec_export(t_env *export, t_command *cmds, t_env *env);
int				exec_unset(t_env *env, char **args, t_env *export);
int				exec_pwd(t_env *env);
int				exec_env(t_env *env);
int				exec_echo(t_command *cmds);
int				exec_cd(t_command *cmd, t_env *env, t_env *export);
int				exec_exit(t_pre_tokens *args, int status);
void			search_in_env_and_replace(t_env *env, char *index, char *str);
char			*search_in_env(t_env *env, char *s);
int				check_if_buil(char *s, t_command *cmds);
int				exec_built(int n, t_command *cmds, t_env *env, t_env *export_head);
//built part end

//execution part end



int	get_len(char *var, t_env *env);
char	*expnd(char *var, t_env *env, int *amb);
char	*get_index(char *idx);
char	*get_value(char *idx, t_env **env);

#endif