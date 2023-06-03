/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_if_buil.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 14:59:29 by bel-kdio          #+#    #+#             */
/*   Updated: 2023/06/01 18:12:51 by bel-kdio         ###   ########.fr       */
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
            newValue = strdup(str);
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
    else if (!cmd->db_args[0] || cmd->db_args[0] == "~")
    {
        search_in_env_and_replace(env, "OLDPWD", search_in_env(env, "PWD"));
        search_in_env_and_replace(export, "OLDPWD", search_in_env(export, "PWD"));
        chdir(search_in_env(env, "HOME"));
        search_in_env_and_replace(env, "OLDPWD", search_in_env(env, "PWD"));
        search_in_env_and_replace(export, "PWD", getcwd(NULL, 0));
    }
}

void exec_echo(t_command *cmds, t_env *env)
{
    t_command *cmds_tmp;
    int i;

    cmds_tmp = cmds;
    if ((cmds_tmp->db_args[0]) && (ft_strncmp(cmds_tmp->db_args[0], "-n", 3) == 0))
    {

        i = 1;
        while (cmds_tmp->db_args[i])
        {
            if (cmds_tmp->db_args[i + 1] == NULL)
            {
                printf("%s", cmds_tmp->db_args[i]);
            }
            else
                printf("%s ", cmds_tmp->db_args[i]);
            i++;
        }
    }
    else
    {
        i = 0;
        while (cmds_tmp->db_args[i])
        {
            printf("%s ", cmds_tmp->db_args[i]);
            i++;
        }
        printf("\n");
    }
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
}

void exec_export(t_env *export, t_command *cmds)
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
            printf("%s", exp->index);
            if(!exp->value)
                printf("\n");
            if(exp->value)
            {
                printf("=");
                printf("%s\n", exp->value);
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
                str = ft_substr(cmd->db_args[i],j+1,ft_strlen(cmd->db_args[i]));
                str = ft_strjoin("\"",str);
                //problem here is segmente
                exit(0);
                str = ft_strjoin(str,"\"");
                new_node = ft_lstnew_env(ft_substr(cmd->db_args[i],0,j),str);
                ft_lstadd_back_env(&exp,new_node);
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
    if (n == 1)
    {
        exec_cd(cmds, env, export);
        return (1);
    }
    else if (n == 2)
    {
        exec_echo(cmds, env);
        return (2);
    }
    else if (n == 6)
    {
        exec_env(env);
        return (6);
    }
    else if (n == 4)
    {
        exec_export(export, cmds);
        return (4);
    }
    else if (n = 3)
    {
        exec_pwd(env);
        return (3);
    }
    // else if (ft_strncmp(s, "exit", 5) == 0)
    //     return (5);
    return (0);
}

int check_if_buil(char *s, t_command *cmds)
{
    int i;
    t_command *tmp;

    tmp = cmds;
    i = 0;

    while (tmp)
    {
        i++;
        tmp = tmp->next;
    }
    tmp = cmds;
    if (ft_strncmp(s, "cd", 3) == 0 && i == 1)
        return (1);
    else if (ft_strncmp(s, "echo", 5) == 0)
        return (2);
    else if (ft_strncmp(s, "pwd", 4) == 0)
        return (3);
    else if (ft_strncmp(s, "export", 7) == 0)
        return (4);
    else if (ft_strncmp(s, "exit", 5) == 0)
        return (5);
    else if (ft_strncmp(s, "env", 4) == 0)
        return (6);
    else if (ft_strncmp(s, "export", 7) == 0)
        return (7);

    return (0);
}
