#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "libft/libft.h"

typedef struct s_cmd {
    char* data1;
    char** data2;
    char* path;
    struct s_cmd* next;
} t_cmd;

char    ***convert_linked_list_to_tr_p(t_cmd *all_cmd);
void    exec(char ***all_cmd, t_cmd *head, char **envp);

