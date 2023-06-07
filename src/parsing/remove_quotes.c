/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabrahm <ylabrahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 15:22:46 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/06/07 18:29:47 by ylabrahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char *get_index(char *string)
{
    int    i;

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

char	*get_value(char *index, t_env **head_env)
{
	t_env	*node;

	node = *head_env;
	while (node)
	{
		if (ft_strncmp(index, node->index, ft_strlen(node->index)) == 0 && (ft_strlen(index) == ft_strlen(node->index)))
			return (ft_strdup(node->value));
		node = node->next;
	}
	return (ft_strdup(""));
}

void	four_free(char **token, char **suffix, char **env_index, char **env_value)
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

char	*trim_value(char *value)
{
	char	*trimed;
	char	*ret;
	int		i;
	int		j;

	trimed = ft_strtrim(value, " \t");
	ret = malloc(sizeof(char) * ft_strlen(trimed) + 1);
	i = 0;
	j = 0;
	while (trimed[i])
	{
		if (trimed[i] == ' ')
		{
			ret[j] = trimed[i];
			i++;
			j++;
			while (trimed[i] == ' ')
				i++;
		}
		ret[j++] = trimed[i++];
	}
	return (ret);
}

char    *get_new_token(char **token, char *new, t_env *head_env, int i)
{
	char	*env_index;
	int		len_to;
	char	*env_value;
	char	*suffix;

    env_index = get_index(&(*token)[i + 1]);
    if (env_index[0] == '?')
        env_value = ft_itoa(globals.exit_status);
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

int    is_valid_variable(char after_dollar)
{
    return ((ft_isalnum(after_dollar)) || (after_dollar == '_') || (after_dollar == '?'));
}

void	expand_loop(char **token, int in_single_quote, int in_double_quote, int i, t_env *head_env)
{
	char	*new;

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
				return ;
			new = get_new_token(token, new, head_env, i);
			(*token) = ft_strdup(new);
			free(new);
		}
	}
}

char	*expand_variable(char *token, t_env *head_env)
{
	int in_single_quote;
	int in_double_quote;
	int	i;

	i = -1;
	in_single_quote = 0;
	in_double_quote = 0;
	expand_loop(&token, in_single_quote, in_double_quote, i, head_env);
	return (token);
}

int	set_up_remove_vars(int *j, int *in_single, int *in_double)
{
	(*j) = 0;
	(*in_single) = 0;
	(*in_double) = 0;
	return (0);
}

int	contains_quotes(char *content)
{
	int	i;

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

void	set_node_type(t_pre_tokens **head, int contain_quotes)
{
	int				i;
	t_pre_tokens	*node;

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

char    *remove_quote(t_pre_tokens *node)
{
    char    *copy;
    int        i;
    int        j;
    int        in_single;
    int        in_double;
    int        contain_quotes;

    i = set_up_remove_vars(&j, &in_single, &in_double);
    copy = malloc(ft_strlen(node->content) + 1);
    while (node->content[i])
    {
        if (node->content[i] == '\'' && !(in_double))
            in_single = !(in_single);
        else if (node->content[i] == '\"' && !(in_single))
            in_double = !(in_double);
        else
            copy[j++] = node->content[i];
        i++;
    }
    copy[j] = '\0';
    free(node->content);
    return (copy);
}

t_pre_tokens	*ft_remove_quotes(t_pre_tokens **head, t_env *head_env)
{
    t_pre_tokens	*node;
    t_pre_tokens	*new_node;
    t_pre_tokens	*new_head;
    t_pre_tokens	**new_head_ix;
    char			*all_data;

    node = *head;
    while (node)
    {
        set_node_type(&node, contains_quotes(node->content));
        node->contain_quotes = contains_quotes(node->content);
		if (!(node->prev))
            node->content = expand_variable(node->content, head_env);
        else if (!(node->prev->type == TYPE_RED_HER))
            node->content = expand_variable(node->content, head_env);
        // node->content = remove_quote(node);
        node = node->next;
    }
	all_data = malloc(sizeof(char));
	all_data[0] = '\0';
	node = *head;
	while (node)
	{
		all_data = ft_strjoin(all_data, node->content);
		all_data = ft_strjoin(all_data, " ");
		node = node->next;
	}
	new_head = ft_tokenizer(all_data);
	new_head_ix = &new_head;
	new_node = *new_head_ix;
	while (new_node)
    {
        set_node_type(&new_node, contains_quotes(new_node->content));
        new_node->contain_quotes = contains_quotes(new_node->content);
		if (!(new_node->prev))
            new_node->content = expand_variable(new_node->content, head_env);
        else if (!(new_node->prev->type == TYPE_RED_HER))
            new_node->content = expand_variable(new_node->content, head_env);
        new_node->content = remove_quote(new_node);
        new_node = new_node->next;
    }
	return (*new_head_ix);
	// printf_linked(*new_head_ix);exit(0);
	// head = new_head_ix;
}
