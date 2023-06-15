/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kdio <bel-kdio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 19:01:18 by ylabrahm          #+#    #+#             */
/*   Updated: 2023/06/15 12:53:28 by bel-kdio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void    reset_here(char **herdoc)
{
    free(*herdoc);
    *herdoc = ft_calloc(1, 1);
}

void    join_herdoc(char **herdoc, char *string)
{
    *herdoc = ft_strjoin(*herdoc, string);
}

int    is_delimiter(char *del, char *content)
{

    if (ft_strncmp(content, del, (ft_strlen(content))) == 0)
        return (1);
    return (0);
}

int    ft_read_heredoc(t_command **command_ix)
{
    t_command        *command;
    t_pre_tokens    *herdoc;
    char            *string;
    char            *all;
    int             pipe_hd[2];

    command = *command_ix;
    string = 0;
    herdoc = command->herdoc_files;
    if (herdoc)
        command->here_doc_data = ft_calloc(1, 1);
    while (herdoc)
    {
        if(isatty(0))
            ft_putstr_fd("> ",1);
        string = get_next_line(0);
        if(!string)
            break;
        ft_strtrim(string, "\n");
        if (is_delimiter(string, herdoc->content))
        {
            herdoc = herdoc->next;
            if (herdoc)
                reset_here(&(command->here_doc_data));
        }
        else
            join_herdoc(&command->here_doc_data, string);
        free(string);
    }
     if (pipe(pipe_hd) == -1) {
        return 0 ;
    }
       if (pipe_hd[1] != -1 && command->here_doc_data)
            write(pipe_hd[1], command->here_doc_data, ft_strlen(command->here_doc_data));
        close(pipe_hd[1]);
        return pipe_hd[0];
}