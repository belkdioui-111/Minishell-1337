/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabrahm <ylabrahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 15:22:46 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/06/13 20:09:01 by ylabrahm         ###   ########.fr       */
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
			if (in_double_quote || ((!in_double_quote) && (*token)[i - 1] && (*token)[i - 1] == '='))
			{
				if (!((*token)[i + 1]))
					return;
				new = get_new_token(token, new, head_env, i);
				(*token) = ft_strdup(new);
				free(new);
			}
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

t_sub expand_variable_2(t_pre_tokens **node_ix, t_env *head_env)
{
	t_token_list token_list;
	char *str;
	int start;
	int i;
	int len;
	char *token;
	int inQuotes;
	int singleq;
	int doubleq;

	// ***********************************************************************************
	token_list.count = 0;
	token_list.tokens = NULL;
	str = ft_strdup((*node_ix)->content);
	len = ft_strlen(str);
	token = NULL;
	inQuotes = 0;
	singleq = 0;
	doubleq = 0;
	i = 0;
	while (i < len)
	{
		if (str[i] == ' ' && !singleq && !doubleq)
		{
			if (token != NULL)
			{
				add_token(&token_list, token);
				token = NULL;
			}
			i++;
			continue;
		}
		if (str[i] == '\'' && !singleq)
			singleq = !singleq;

		if (str[i] == '\"' && !doubleq)
			doubleq = !doubleq;
		/*
		if (str[i] == '\'' || str[i] == '\"')
		{
			if (inQuotes && str[i] == inQuotes)
			{
				inQuotes = 0;
			}
			else if (!inQuotes)
			{
				inQuotes = str[i];
			}
			i++;
			continue;
		}
		*/
		if (str[i] == '$' && !singleq)
		{
			if (token != NULL)
			{
				add_token(&token_list, token);
				token = NULL;
			}
			int j = i + 1;
			while (j < len && (ft_isalnum(str[j]) || str[j] == '_'))
			{
				j++;
			}
			if (j > i + 1)
			{
				int varLen = j - i - 1;
				token = malloc((varLen + 2) * sizeof(char));
				token[0] = '$';
				ft_strncpy(token + 1, str + i + 1, varLen);
				token[varLen + 1] = '\0';
				add_token(&token_list, token);
				token = NULL;
				i = j;
				continue;
			}
		}
		if (token == NULL)
		{
			token = malloc((len - i + 1) * sizeof(char));
			token[0] = '\0';
		}
		int tokenLen = ft_strlen(token);
		char *newToken = malloc((tokenLen + 2) * sizeof(char));
		strcpy(newToken, token);
		newToken[tokenLen] = str[i];
		newToken[tokenLen + 1] = '\0';
		free(token);
		token = newToken;
		i++;
	}
	if (token != NULL)
		add_token(&token_list, token);
	// i = 0;
	// while (i < token_list.count)
	// {
	// 	printf("%s\n", token_list.tokens[i]);
	// 	i++;
	// }
	// ***********************************************************************************
	i = 0;
	while (i < token_list.count)
	{
		if (token_list.tokens[i][0] == '$')
		{
			token_list.tokens[i] = expand_variable(token_list.tokens[i], head_env, 2);
			non_printable(&(token_list.tokens[i]), 127);
		}
		i++;
	}
	i = 0;
	char *new_all;
	new_all = ft_calloc(1, 1);
	while (i < token_list.count)
	{
		new_all = ft_strjoin(new_all, token_list.tokens[i]);
		i++;
	}
	// printf("%s\n", new_all);exit(0);
	char **new_splited;
	new_splited = ft_split(new_all, 127);
	t_sub returned;
	returned.type = TYPE_ARG;
	returned.sub = new_splited;
	return (returned);
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

t_pre_tokens *put_middle(t_pre_tokens **middle_ix, t_pre_tokens **node_ix)
{
	t_pre_tokens *node;
	t_pre_tokens *middle;
	t_pre_tokens *temp_last;

	node = *node_ix;
	middle = *middle_ix;
	temp_last = node->next;
	(*node_ix) = middle;
	while (node->next)
		node = node->next;
	node->next = temp_last;
	return (*node_ix);
}

t_sub get_sub_from_node(t_pre_tokens **node_ix)
{
	char **sub;
	t_sub returned;

	returned.type = (*node_ix)->type;
	returned.sub = ft_split((*node_ix)->content, 30);
	return (returned);
}

void ft_remove_quotes(t_pre_tokens **head, t_env *head_env)
{
	t_pre_tokens	*node;
	char			**splited;
	int 			i;

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
	// --------------------------------------------------------
	node = *head;
	while (node)
	{
		node->contain_quotes = contains_quotes(node->content);
		if (node->type == TYPE_ARG)
		{
			if ((!(node->prev)) || (node->prev->type == TYPE_ARG || node->prev->type == TYPE_RED_PIP))
				node->sub = expand_variable_2(&node, head_env);
			else
				node->sub = get_sub_from_node(&node);
		}
		else
			node->sub = get_sub_from_node(&node);
		node = node->next;
	}
}
