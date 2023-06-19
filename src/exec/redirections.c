/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 10:07:17 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/19 20:09:24 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	perr(char *arg, char *msg, int ret)
{
	ft_putstr_fd("minishell: ", 2);
	if (arg)
		ft_putstr_fd(arg, 2);
	if (arg)
		ft_putstr_fd(": ", 2);
	ft_putendl_fd(msg, 2);
	return (ret);
}

int	get_len(char *var, t_env *env)
{
	int	len;
	int	i;

	len = 0;
	i = 0;
	while (var[i])
	{
		if (var[i] == '\'' && ++i)
		{
			while (var[i] && var[i] != '\'')
			{i++;len++;}
			i++;
		}
		else if (var[i] == '"' && ++i)
		{
			while (var[i] != '"')
			{
				if (var[i] == '$' && ++i)
				{
					char *key = get_index(var + i);
					char *value = get_value(key, &env);
					i += ft_strlen(key);
					len += ft_strlen(value);
					free(key);
					free(value);
				}
				else
				{i++;len++;}
			}
			i++;
		}
		else if (var[i] == '$' && ++i)
		{
			char *key = get_index(var + i);
			char *value = get_value(key, &env);
			i += ft_strlen(key);
			len += ft_strlen(value);
			free(key);
			free(value);
		}
		else
		{i++;len++;}
	}
	return (len);
}

char	*expand_redirs(char *var, t_env *env)
{
	char	*exp;
	char	**arr;
	int		n;

	exp = expnd(var, env, &n);
	arr = ft_split(exp, 2);
	free(exp);
	n = 0;
	while (arr[n])
		n++;
	if (n > 1)
		return (free_double(arr), NULL);
	else if (n == 0 && !ft_strchr(var, '"') && !ft_strchr(var, '\''))
		return (free_double(arr), NULL);
	exp = arr[0];
	if (arr[0] == NULL)
		exp = ft_strdup("");
	free(arr);
	return (exp);
}

char	*expnd(char *var, t_env *env, int *amb)
{
	char	*exp;
	int		i;
	int		j;

	exp = ft_calloc(get_len(var, env) + 1, 1);
	i = 0;
	j = 0;
	while (var[i])
	{
		if (var[i] == '\'' && ++i)
		{
			while (var[i] && var[i] != '\'')
			{exp[j++] = var[i++];}
			i++;
		}
		else if (var[i] == '"' && ++i)
		{
			while (var[i] != '"')
			{
				if (var[i] == '$' && ++i)
				{
					char *key = get_index(var + i);
					char *value = get_value(key, &env);
					i += ft_strlen(key);
					ft_memcpy(&exp[j], value, ft_strlen(value));
					j += ft_strlen(value);
					free(key);
					free(value);
				}
				else
				{exp[j++] = var[i++];}
			}
			i++;
		}
		else if (var[i] == '$' && ++i)
		{
			char *key = get_index(var + i);
			char *value = get_value(key, &env);
			i += ft_strlen(key);
			for (int k = 0; value[k]; k++)
				if (value[k] == ' ' || value[k] == '\t')
					value[k] = 2;
			ft_memcpy(&exp[j], value, ft_strlen(value));
			j += ft_strlen(value);
			free(key);
			free(value);
		}
		else
		{exp[j++] = var[i++];}
	}
	// char **arr = ft_split(exp, 2);
	// i = 0;
	// while (arr[i])
	// 	i++;
	// if (i > 1)
	// 	*amb = 1;
	// else if (i == 0 && !ft_strchr(var, '"') && !ft_strchr(var, '\''))
	// 	*amb = 1;
	// if (*amb == 1)
	// 	*exp = 1;
	return (exp);
}

int	output(t_pre_tokens *out, t_env *env)
{
	char	*exp;
	int		amb;
	int		fd;

	amb = 0;
	while (out)
	{
		exp = expand_redirs(out->content, env);
		if (exp == NULL)
			return (glob.exit_status = pr_err("minishell: ", NULL,"ambiguous redirect\n", 1));
		if (out->type == TYPE_RED_OUT)
			fd = open(exp, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (out->type == TYPE_RED_APP)
			fd = open(exp, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
			return (glob.exit_status = perr(exp, strerror(errno), 1), free(exp), 1);
		dup2(fd, 1);
		close(fd);
		out = out->next;
		free(exp);
	}
	return (0);
}

/*

pipe(fds);

write(fds);

in->content = (char *)fd[0];

*/

int	input(t_pre_tokens *in, t_env *env)
{
	char	*exp;
	int		amb;
	int		fd;

	amb = 0;
	while (in)
	{
		// if (in->type == TYPE_RED_HER)
		// {
		// 	dup2((int)in->content, 0);
		// 	close;
		//	in = in->next;
		//	continue;
		// }
		// exp = expand_red(in, &amb, env);
		exp = expand_redirs(in->content, env);
		if (exp == NULL)
			return (glob.exit_status = pr_err("minishell: ", NULL,"ambiguous redirect\n", 1));
		fd = open(exp, O_RDONLY);
		if (fd == -1)
			return (glob.exit_status = perr(exp, strerror(errno), 1), free(exp), 1);
		dup2(fd, 0);
		close(fd);
		in = in->next;
	}
	return (0);
}

/*
int	output(t_pre_tokens *out)
{
	int	out_file;

	while (out)
	{
		if (ft_strlen(out->content) == 0)
		{
			glob.exit_status = pr_err("minishell: ", out->content,
					": No such file or directory\n", 1);
			return 1;
		}
		if (out->type == TYPE_RED_OUT)
			out_file = open(out->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (out->type == TYPE_RED_APP)
			out_file = open(out->content, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
		{
			out = out->next;
			continue ;
		}
		if (out_file != -1)
		{	
                dup2(out_file, STDOUT_FILENO);
				 close(out_file);
		}
		else
		{
			if (access(out->content, R_OK) != 0)
            {
                glob.exit_status = pr_err("minishell: ", out->content,
                                          ": Permission denied\n", 1);
                return 1;
            }
			glob.exit_status = pr_err("minishell: ", out->content,
					": No such file or directory\n", 1);
		}
		out = out->next;
	}
	return (0);
}

*/

/*
int	input(t_pre_tokens *in)
{
	int	in_file;

	while (in)
	{
		if (ft_strlen(in->content) == 0)
		{
			glob.exit_status = pr_err("minishell: ", in->content,
					": No such file or directory\n", 1);
			return 1;
		}
		in_file = open(in->content, O_RDONLY);
		if (in_file != -1)
		{
            dup2(in_file, STDIN_FILENO);
            close(in_file);
		}
		else
		{
			if (access(in->content, R_OK) != 0)
            {
                glob.exit_status = pr_err("minishell: ", in->content,
                                          ": Permission denied\n", 1);
                return 1;
            }
			glob.exit_status = pr_err("minishell: ", in->content,
					": No such file or directory\n", 1);
		}
		in = in->next;
	}
	return (0);
}
*/

int	redirection(t_command *head, t_env *env)
{
	t_pre_tokens	*out;
	t_pre_tokens	*in;
	int				ret;
	int				cmdn;
	
	ret = 0;
	cmdn = 0;
	if (head->herdoc_files && head->cmd)
	{
		dup2(head->pipe_hd, 0);
		close(head->pipe_hd);
	}
	out = head->output_files;
	in = head->input_files;
	if (head->output_files)
		if (output(out, env))
			return (1);
	if (head->input_files)
		if (input(in, env))
			return (1);
	if(head->cmd == NULL)
		return (2);
	return (0);
}
