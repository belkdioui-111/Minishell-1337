/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabrahm <ylabrahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 15:22:46 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/06/19 11:20:18 by ylabrahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <string.h>

char *get_index(char *string)
{
	int i;

	i = 0;
	if ((string[i]) && ((string[i] == '?') || (string[i] == '@')))
		return (ft_substr(string, 0, i + 1));
	if ((string[i]) && (ft_isdigit(string[i])))
		return (ft_substr(string, 0, i + 1));
	while (1)
	{
		if (!(string[i] && (ft_isalnum(string[i]) || string[i] == '_')))
			break;
		i++;
	}
	return (ft_substr(string, 0, i));
}

char *get_value(char *index, t_env **head_env)
{
	t_env *node;

	node = *head_env;
	while (node)
	{
		if (ft_strncmp(index, node->index, ft_strlen(node->index)) == 0 && (ft_strlen(index) == ft_strlen(node->index)))
			return (ft_strdup(node->value));
		node = node->next;
	}
	return (ft_strdup(""));
}

void four_free(char **token, char **suffix, char **env_index, char **env_value)
{
	if (*token)
		free(*token);
	if (*suffix)
		free(*suffix);
	if (*env_index)
		free(*env_index);
	if (*env_value)
		free(*env_value);
}

char *get_new_token(char **token, char *new, t_env *head_env, int i)
{
	char *env_index;
	int len_to;
	char *env_value;
	char *suffix;

	env_index = get_index(&(*token)[i + 1]);
	if (env_index[0] == '?')
		env_value = ft_itoa(glob.exit_status);
	else
		env_value = get_value(env_index, &head_env);
	new = ft_substr((*token), 0, i);
	new = ft_strjoin(new, env_value);
	len_to = ft_strlen(&(*token)[i + 1]) - ft_strlen(env_index);
	suffix = ft_substr(&(*token)[i + 1], ft_strlen(env_index), len_to);
	new = ft_strjoin(new, suffix);
	four_free(token, &suffix, &env_index, &env_value);
	return (new);
}

int is_valid_variable(char after_dollar)
{
	return ((ft_isalnum(after_dollar)) || (after_dollar == '_') || (after_dollar == '?'));
}

void expand_loop_1(char **token, int in_single_quote, int in_double_quote, int i, t_env *head_env)
{
	t_pre_tokens	*head;
	char			*new;

	new = NULL;
	while ((*token)[++i])
	{
		if (((*token)[i] == '\'') && (!in_double_quote))
			in_single_quote = (!in_single_quote);
		if (((*token)[i] == '\"') && (!in_single_quote))
			in_double_quote = (!in_double_quote);
		if (((*token)[i] == '$') && (in_double_quote) && (!in_single_quote))
		{
			if (!((*token)[i + 1]))
				return;
			new = get_new_token(token, new, head_env, i);
			if ((ft_strlen(new) == 0))
			{
				*token = ft_strdup("");
				return ;
			}
			(*token) = ft_strdup(new);
			free(new);
		}
	}
}

void expand_loop_2(char **token, int in_single_quote, int in_double_quote, int i, t_env *head_env)
{
	t_pre_tokens *head;
	char *new;

	new = NULL;
	while ((*token)[++i])
	{
		if (((*token)[i] == '\'') && (!in_double_quote))
			in_single_quote = (!in_single_quote);
		if (((*token)[i] == '\"') && (!in_single_quote))
			in_double_quote = (!in_double_quote);
		if ((*token)[i] == '$' && (!in_single_quote))
		{
			if (!((*token)[i + 1]))
				return;
			new = get_new_token(token, new, head_env, i);
			if ((ft_strlen(new) == 0))
			{
				*token = ft_strdup("");
				return ;
			}
			(*token) = ft_strdup(new);
			free(new);
		}
	}
}

char *expand_variable(char *token, t_env *head_env, int state)
{
	int in_single_quote;
	int in_double_quote;
	int i;

	i = -1;
	in_single_quote = 0;
	in_double_quote = 0;
	if (state == 1)
		expand_loop_1(&token, in_single_quote, in_double_quote, i, head_env);
	else
		expand_loop_2(&token, in_single_quote, in_double_quote, i, head_env);
	return (token);
}

typedef struct
{
	char **tokens;
	int count;
} t_token_list;

void add_token(t_token_list *list, char *token)
{
	char **new_tokens;
	int i;

	i = 0;
	new_tokens = malloc((list->count + 1) * sizeof(char *));
	while (i < list->count)
	{
		new_tokens[i] = list->tokens[i];
		i++;
	}
	new_tokens[list->count] = ft_strdup(token);
	free(list->tokens);
	list->tokens = new_tokens;
	list->count++;
}

void free_tokens(t_token_list *list)
{
	int i;

	i = 0;
	while (i < list->count)
		free(list->tokens[i++]);
	free(list->tokens);
	list->count = 0;
}

void non_printable(char **token, int special)
{
	int i;

	i = 0;
	while ((*token)[i])
	{
		if ((*token)[i] == ' ' || (*token)[i] == '\t')
			(*token)[i] = special;
		i++;
	}
}

typedef struct s_expand_variable
{
	t_token_list	token_list;
	char			*str;
	char			*token;
	char			*newToken;
	int				i;
	int				len;
	int				singleq;
	int				doubleq;
}	t_expand_variable;

void	free_exp(t_expand_variable *var)
{
	int	i;

	free(var->str);
	free(var->token);
	i = 0;
	while (i < var->token_list.count)
	{
		free(var->token_list.tokens[i]);
		i++;
	}
	free(var->token_list.tokens);
}

t_sub	expand_variable_2_tool(t_expand_variable *var, t_env *head_env)
{
	int		i;
	char	*new_all;
	char	**new_splited;
	t_sub	returned;

	i = 0;
	while (i < var->token_list.count)
	{
		if (var->token_list.tokens[i][0] == '$')
		{
			var->token_list.tokens[i] = expand_variable(var->token_list.tokens[i], head_env, 2);
			non_printable(&(var->token_list.tokens[i]), 127);
		}
		i++;
	}
	i = 0;
	new_all = ft_calloc(1, 1);
	while (i < var->token_list.count)
		new_all = ft_strjoin(new_all, var->token_list.tokens[i++]);
	new_splited = ft_split(new_all, 127);
	free(new_all);
	returned.type = TYPE_ARG;
	returned.sub = new_splited;
	free_exp(var);
	return (returned);
}


char	*ft_getoken(t_expand_variable *var)
{
	char	*new_token;

	new_token = malloc((ft_strlen(var->token) + 2) * sizeof(char));
	ft_strcpy(new_token, var->token);
	new_token[ft_strlen(var->token)] = var->str[var->i];
	new_token[ft_strlen(var->token) + 1] = '\0';
	free(var->token);
	(var->i)++;
	return (new_token);
}

void	ft_check_if_null(t_expand_variable *var)
{
	if (var->token == NULL)
	{
		var->token = malloc((var->len - var->i + 1) * sizeof(char));
		(var->token)[0] = '\0';
	}
}

void	check_if_space(char **token, t_token_list *token_list, int *i)
{
	if (*token != NULL)
	{
		add_token(token_list, *token);
		*token = NULL;
	}
	(*i)++;
}


int	is_a_dollar(t_expand_variable *var)
{
	int	varLen;
	int	j;

	if (var->token != NULL)
	{
		add_token(&(var->token_list), var->token);
		var->token = NULL;
	}
	j = var->i + 1;
	while (j < var->len && (ft_isalnum(var->str[j]) || var->str[j] == '_'))
		j++;
	if (j > var->i + 1)
	{
		varLen = j - var->i - 1;
		var->token = malloc((varLen + 2) * sizeof(char));
		var->token[0] = '$';
		ft_strncpy(var->token + 1, var->str + var->i + 1, varLen);
		var->token[varLen + 1] = '\0';
		add_token(&(var->token_list), var->token);
		var->token = NULL;
		var->i = j;
		return (1);
	}
	return (0);
}

void	ft_init_ex_2_vars(t_expand_variable *var, char *content)
{
	var->token_list.count = 0;
	var->token_list.tokens = NULL;
	var->str = ft_strdup(content);
	var->len = ft_strlen(var->str);
	var->token = NULL;
	var->singleq = 0;
	var->doubleq = 0;
	var->i = 0;
}

t_sub expand_variable_2(t_pre_tokens **node_ix, t_env *head_env)
{
	t_expand_variable	var;
	t_sub				ret;

	ft_init_ex_2_vars(&var, (*node_ix)->content);
	while (var.i < var.len)
	{
		if ((var.str[var.i] == ' ') && (!(var.singleq)) && (!(var.doubleq)))
		{
			check_if_space(&(var.token), &(var.token_list), &(var.i));
			continue;
		}
		if ((var.str[var.i]) == '\'' && (!(var.doubleq)))
			(var.singleq) = !(var.singleq);
		if (var.str[var.i] == '\"' && !(var.singleq))
			var.doubleq = !(var.doubleq);
		if (var.str[var.i] == '$' && !(var.singleq))
		{
			if (is_a_dollar(&var))
				continue;
		}
		ft_check_if_null(&var);
		var.token = ft_getoken(&var);
	}
	if (var.token != NULL)
		add_token(&(var.token_list), var.token);
	ret = (expand_variable_2_tool(&var, head_env));
	return (ret);
}

int set_up_remove_vars(int *j, int *in_single, int *in_double)
{
	(*j) = 0;
	(*in_single) = 0;
	(*in_double) = 0;
	return (0);
}

int contains_quotes(char *content)
{
	int i;

	i = 0;
	if (!content)
		return (0);
	while (content[i])
	{
		if (content[i] == '\'' || content[i] == '\"')
			return (1);
		i++;
	}
	return (0);
}

void set_node_type(t_pre_tokens **head, int contain_quotes)
{
	int i;
	t_pre_tokens *node;

	i = 0;
	node = *head;
	node->type = TYPE_ARG;
	if (contain_quotes == 0)
	{
		if (ft_strlen(node->content) == 1)
		{
			if (ft_strncmp(node->content, "<", 1) == 0)
				node->type = TYPE_RED_IN;
			if (ft_strncmp(node->content, ">", 1) == 0)
				node->type = TYPE_RED_OUT;
			if (ft_strncmp(node->content, "|", 1) == 0)
				node->type = TYPE_RED_PIP;
		}
		else
		{
			if (ft_strncmp(node->content, ">>", 2) == 0)
				node->type = TYPE_RED_APP;
			if (ft_strncmp(node->content, "<<", 2) == 0)
				node->type = TYPE_RED_HER;
		}
	}
}

char *remove_quote(char *content)
{
	char *copy;
	int i;
	int j;
	int in_single;
	int in_double;
	int contain_quotes;

	i = set_up_remove_vars(&j, &in_single, &in_double);
	copy = malloc(ft_strlen(content) + 1);
	while (content[i])
	{
		if (content[i] == '\'' && !(in_double))
			in_single = !(in_single);
		else if (content[i] == '\"' && !(in_single))
			in_double = !(in_double);
		else
			copy[j++] = content[i];
		i++;
	}
	copy[j] = '\0';
	free(content);
	return (copy);
}

t_sub get_sub_from_node(t_pre_tokens **node_ix)
{
	char **sub;
	t_sub returned;

	returned.type = (*node_ix)->type;
	returned.sub = ft_split((*node_ix)->content, 30);
	return (returned);
}

void	ft_remove_quotes_2(t_pre_tokens **head, t_env *head_env)
{
	t_pre_tokens	*node;

	node = *head;
	while (node)
	{
		node->contain_quotes = contains_quotes(node->content);
		if (node->type == TYPE_ARG)
		{
			if ((!(node->prev)) || (node->prev->type == TYPE_ARG || node->prev->type == TYPE_RED_PIP))
			{
				node->sub = expand_variable_2(&node, head_env);
			}
			else
			{
				node->sub = get_sub_from_node(&node);
			}
		}
		else
		{
			node->sub = get_sub_from_node(&node);
		}
		node = node->next;
	}
}

void ft_remove_quotes(t_pre_tokens **head, t_env *head_env)
{
	t_pre_tokens	*node;

	node = *head;
	while (node)
	{
		set_node_type(&node, contains_quotes(node->content));
		node->contain_quotes = contains_quotes(node->content);
		if (!(node->prev))
			node->content = expand_variable(node->content, head_env, 1);
		else if (node->prev->type == TYPE_ARG)
			node->content = expand_variable(node->content, head_env, 1);
		node = node->next;
	}
	ft_remove_quotes_2(head, head_env);
}
