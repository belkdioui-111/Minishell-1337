#include "test.h"

char	**get_path(char **envp)
{
	int		i;
	char	**path;
	int		check;

	i = 0;
	check = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp("PATH", envp[i], 4) == 0)
		{
			check = 1;
			break ;
		}
		i++;
	}
	if (check == 0)
		envp[i] = "/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin";
	envp[i] = ft_strtrim(envp[i], "PATH=");
	return (path = ft_split(envp[i], ':'));
}

t_cmd* createNode(char* data1, char** data2) {
	t_cmd* newNode = (t_cmd*)malloc(sizeof(t_cmd));
	newNode->data1 = data1;
	newNode->data2 = data2;
	newNode->next = NULL;
	return newNode;
}

void insertNode(t_cmd** head, char* data1, char** data2) {
	t_cmd* newNode = createNode(data1, data2);
	
	if (*head == NULL) {
		*head = newNode;
	} else {
		t_cmd* temp = *head;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = newNode;
	}
}

int main(int ac, char **av, char **envp) {
	t_cmd* head = NULL;
	
	// Example data
	char* data1_1 = "sleep";
	char* data1_2 = "ls";
	char* data2_1[] = { "5" ,NULL };
	char* data2_2[] = { NULL };
	insertNode(&head, data1_1, data2_1);
	insertNode(&head, data1_2, data2_2);
	// for getting the path
	char **path;
	t_cmd* tmp = head;
	int i;
	while(tmp)
	{
		i = 0;
		path = get_path(envp);
		while (path[i])
		{
			path[i] = ft_strjoin(path[i], "/");
			path[i] = ft_strjoin(path[i], tmp->data1);
			if (access(path[i], F_OK | X_OK) != -1)
			{
				tmp->path = path[i];
				break;
			}
			i++;
		}
		tmp = tmp->next;
	}
	// end 
	char*** all_cmd = convert_linked_list_to_tr_p(head);
	(void) ac;
	(void) av;
	exec(all_cmd, head, envp);
	// int i = 0;
	// i=0;
	// while (all_cmd[i] != NULL) {
	// 	char** stringArray = all_cmd[i];
	// 	while (*stringArray != NULL) {
	// 		printf("%s ", *stringArray);
	// 		stringArray++;
	// 	}
	// 	printf("\n");
	// 	i++;
	// }
	
	// TODO: Free allocated memory
	
	return 0;
}