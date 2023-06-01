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

void    exec_cd(t_command *cmd)
{
    if(access(cmd->db_args[0],F_OK) == 0)
    {
        chdir(cmd->db_args[0]);
    }
}

int    exec_built(int n, t_command *cmds)
{
    if (n == 1)
    {
        exec_cd(cmds);
        return(1);
    }
    // else if (ft_strncmp(s, "echo", 5) == 0)
    // {
    //     // exec_echo(tmp);
    //     return (2);
    // }
    // else if (ft_strncmp(s, "pwd", 4) == 0)
    // {
    //     // exec_pwd(tmp);
    //     return (3);
    // }
    // else if (ft_strncmp(s, "export", 7) == 0)
    //     return (4);
    // else if (ft_strncmp(s, "exit", 5) == 0)
    //     return (5);
    return (0);
}

int    check_if_buil(char *s, t_command *cmds)
{
    int i;
    t_command *tmp;

    tmp = cmds;
    i=0;

    while(tmp)
    {
        i++;
        tmp=tmp->next;
    }
    tmp = cmds;
    if (ft_strncmp(s, "cd", 3) == 0 && i == 1)
    {
        return(1);
    }
    else if (ft_strncmp(s, "echo", 5) == 0)
    {
        // exec_echo(tmp);
        return (2);
    }
    else if (ft_strncmp(s, "pwd", 4) == 0)
    {
        // exec_pwd(tmp);
        return (3);
    }
    else if (ft_strncmp(s, "export", 7) == 0)
        return (4);
    else if (ft_strncmp(s, "exit", 5) == 0)
        return (5);
    return (0);
}
