/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_if_buil.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 14:59:29 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/06 11:35:30 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_env	*ft_lstlast_env(t_env *lst)
{
	t_env	*temp;

	if (!lst)
		return (NULL);
	temp = lst;
	while (temp->next != NULL)
	{
		temp = temp->next;
	}
	return (temp);
}
void	ft_lstadd_back_env(t_env **lst, t_env *new)
{
	t_env	*temp;

	if (*lst != NULL)
	{
		temp = ft_lstlast_env(*lst);
		temp->next = new;
	}
	else
		*lst = new;
}

t_env	*ft_lstnew_env(char *index, char *value)
{
	t_env	*new_node;

	new_node = (t_env *) malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->index = index;
    new_node->value = value;
	new_node->next = NULL;
	return (new_node);
}


void search_in_env_and_replace(t_env *env, char *index, char *str)
{
    t_env *tmp;
    char *newValue;

    tmp = env;
    while (tmp)
    {
        if (ft_strncmp(index, tmp->index, ft_strlen(index)) == 0)
        {
            newValue = ft_strdup(str);
            free(tmp->value);
            tmp->value = newValue;
            return;
        }
        tmp = tmp->next;
    }
}

char *search_in_env(t_env *env, char *s)
{
    t_env *tmp;

    tmp = env;
    while (tmp)
    {
        if (ft_strncmp(s, tmp->index, ft_strlen(s)) == 0)
            return (tmp->value);
        tmp = tmp->next;
    }
    return (NULL);
}

int     check_numric(char *content)
{
    int i;

    i = 0;
    if (ft_strlen(content) == 0)
        return (1);
    while (content[i])
    {
        if (!(ft_isdigit(content[i])))
            return (1);
        i++;
    }
    return (0);
}

void    exec_exit(t_pre_tokens *args, int status)
{
    if (args)
    {
        if (args->next)
        {
            print_error("too many arguments");
            return ;
        }
        else
        {
            if (check_numric(args->content) == 1)
            {
                print_error("numeric argument required");
                status = 255;
            }
            else
                status = ft_atoi(args->content);
        }
    }
    exit(status);
}

void exec_cd(t_command *cmd, t_env *env, t_env *export)
{
    if (access(cmd->db_args[0], F_OK) == 0)
    {
        search_in_env_and_replace(env, "OLDPWD", search_in_env(env, "PWD"));
        search_in_env_and_replace(export, "OLDPWD", search_in_env(export, "PWD"));
        chdir(cmd->db_args[0]);
        search_in_env_and_replace(env, "PWD", getcwd(NULL, 0));
        search_in_env_and_replace(export, "PWD", getcwd(NULL, 0));
    }
    else if (!cmd->db_args[0] || ft_strncmp(cmd->db_args[0], "~", 2) == 0)
    {
        search_in_env_and_replace(env, "OLDPWD", search_in_env(env, "PWD"));
        search_in_env_and_replace(export, "OLDPWD", search_in_env(export, "PWD"));
        chdir(search_in_env(env, "HOME"));
        search_in_env_and_replace(env, "PWD", getcwd(NULL, 0));
        search_in_env_and_replace(export, "PWD", getcwd(NULL, 0));
    }
    else
    {
        ft_putstr_fd("minishell: No such file or directory\n",2);
        globals.exit_status = 1;
    }
}

void    set_zeros(int *ret, int *is_n, int *inv_flag)
{
    *ret = 0;
    *is_n = 0;
    *inv_flag = 0;
}

void    print_new_line(int inv_flag, int is_n)
{
    if (inv_flag)
    {
        printf("\n");
        return ;
    }
    if (!is_n)
        printf("\n");
}

void    print_args(t_pre_tokens *args)
{
    while (args)
    {
        printf("%s", args->content);
        args = args->next;
        if (args)
            printf(" ");
    }
}

void exec_echo(t_command *cmds)
{
    int                ret;
    int                i;
    int                is_n;
    int                inv_flag;
    t_pre_tokens    *args;

    set_zeros(&ret, &is_n, &inv_flag);
    args = cmds->args;
    while (args && args->content[0] == '-')
    {
        i = 0;
        while (args->content[++i])
        {
            if (args->content[i] == 'n')
                is_n = 1;
            else
                inv_flag = 1;
        }
        args = args->next;
    }
    if (inv_flag)
        args = cmds->args;
    print_args(args);
    print_new_line(inv_flag, is_n);
}

void exec_env(t_env *env)
{
    t_env *tmp;

    tmp = env;
    while (tmp)
    {
        printf("%s=", tmp->index);
        printf("%s\n", tmp->value);
        tmp = tmp->next;
    }
}

void exec_pwd(t_env *env)
{
    printf("%s\n", search_in_env(env, "PWD"));
    globals.exit_status=0;
}


void check_and_free_unset(t_env **env, char *args, t_env **export)
{
    t_env *env_tmp = *env;
    t_env *prev_env = NULL;
    t_env *export_tmp = *export;
    t_env *prev_export = NULL;

    while (env_tmp)
    {
        if (ft_strncmp(env_tmp->index, args, ft_strlen(args)) == 0)
        {
            if (prev_env != NULL)
                prev_env->next = env_tmp->next;
            else
                *env = env_tmp->next;

            free(env_tmp->index);
            free(env_tmp->value);
            free(env_tmp);
            env_tmp = (prev_env != NULL) ? prev_env->next : *env;
        }
        else
        {
            prev_env = env_tmp;
            env_tmp = env_tmp->next;
        }
    }

    while (export_tmp)
    {
        if (ft_strncmp(export_tmp->index, args, ft_strlen(args)) == 0)
        {
            if (prev_export != NULL)
                prev_export->next = export_tmp->next;
            else
                *export = export_tmp->next;

            free(export_tmp->index);
            free(export_tmp->value);
            free(export_tmp);
            export_tmp = (prev_export != NULL) ? prev_export->next : *export;
            return;
        }
        else
        {
            prev_export = export_tmp;
            export_tmp = export_tmp->next;
        }
    }
}

void exec_unset(t_env *env, char **args, t_env *export)
{
    int i;

    i = 0;
    while(args[i])
    {
        check_and_free_unset(&env, args[i], &export);
        i++;
    }
}
void exec_export(t_env *export, t_command *cmds, t_env *env)
{
    t_env *exp;
    t_command *cmd;
    int i;
    int j;
    t_env *new_node;
    char *str;

    exp = export;
    cmd = cmds;
    i=0;
    if(!cmds->db_args[0])
    {
        while(exp)
        {
            printf("declare -x ");
            printf("%s", exp->index);
            if(!exp->value)
                printf("\n");
            if(exp->value)
            {
                printf("=");
                printf("\"");
                printf("%s\"\n", exp->value);
            }
            exp = exp->next;
        }
    }
    else
    {
        while(cmd->db_args[i])
        {
            j = 0;
            if(ft_strchr(cmd->db_args[i], '='))
            {
                while(cmd->db_args[i][j])
                {
                    if(cmd->db_args[i][j] == '=')
                    {
                        break;
                    }
                    j++;
                }
                if(!search_in_env(exp, ft_substr(cmd->db_args[i], 0, j)))
                {
                    str = ft_substr(cmd->db_args[i],j+1,ft_strlen(cmd->db_args[i]));
                    new_node = ft_lstnew_env(ft_substr(cmd->db_args[i],0,j),str);
                    ft_lstadd_back_env(&env,new_node);
                    new_node = ft_lstnew_env(ft_substr(cmd->db_args[i],0,j),str);
                    ft_lstadd_back_env(&exp,new_node);
                }
                else
                {
                    search_in_env_and_replace(exp,ft_substr(cmd->db_args[i],0,j),ft_substr(cmd->db_args[i],j+1, ft_strlen(cmd->db_args[i])));
                    search_in_env_and_replace(env,ft_substr(cmd->db_args[i],0,j),ft_substr(cmd->db_args[i],j+1, ft_strlen(cmd->db_args[i])));
                }
            }
            else
            {
                new_node = ft_lstnew_env(ft_substr(cmd->db_args[i],0,ft_strlen(cmd->db_args[i])),NULL);
                ft_lstadd_back_env(&exp,new_node);
            }
            i++;
        }
    }
}

int exec_built(int n, t_command *cmds, t_env *env, t_env *export)
{
    if (n == 1 || n == 11)
    {
        exec_cd(cmds, env, export);
        return (1);
    }
    else if (n == 2 || n == 12)
    {
        exec_echo(cmds);
        return (2);
    }
    else if (n == 6 || n == 16)
    {
        exec_env(env);
        return (6);
    }
    else if (n == 5 || n == 15)
    {
        exec_exit(cmds->args, 0);
        return(5);
    }
    else if (n == 4 || n == 14)
    {
        exec_export(export, cmds, env);
        return (4);
    }
    else if (n == 3 || n == 13)
    {
        exec_pwd(env);
        return (3);
    }
    else if (n == 7 || n == 17)
    {
        exec_unset(env, cmds->db_args, export);
        return (7);
    }
    return (0);
}

int check_if_buil(char *s, t_command *cmds)
{
    int i;
    t_command *tmp;

    i = 0;
    tmp = cmds;
    while (tmp)
    {
        i++;
        tmp = tmp->next;
    }
    tmp = cmds;
    if (ft_strncmp(s, "cd", 3) == 0)
    {
        if(i == 1)
            return (1);
        return(11);
    }
    else if (ft_strncmp(s, "echo", 5) == 0)
    {
      if(i == 1)
            return (2);
        return(12);   
    }
    else if (ft_strncmp(s, "pwd", 4) == 0)
    {
         if(i == 1)
            return (3);
        return(13);
    }
    else if (ft_strncmp(s, "export", 7) == 0)
    {
         if(i == 1)
            return (4);
        return(14);
    }
    else if (ft_strncmp(s, "exit", 5) == 0)
    {
         if(i == 1)
            return (5);
        return(15);
    }
    else if (ft_strncmp(s, "env", 4) == 0)
    {
         if(i == 1)
            return (6);
        return(16);
    }
    else if (ft_strncmp(s, "unset", 6) == 0)
    {
         if(i == 1)
            return (7);
        return(17);
    }

    return (0);
}
