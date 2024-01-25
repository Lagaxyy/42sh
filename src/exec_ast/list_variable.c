#include "../variable/variable.h"
#include "exec_ast.h"

#include <string.h>

void free_list(struct ListVariable *list)
{
    while (list != NULL)
    {
        struct ListVariable *temp = list;
        list = list->next;
        free_var(temp->var);
        free(temp);
    }
}

struct Variable *find_variable(struct ListVariable *list, struct Token *name)
{
    while (list != NULL)
    {
        if (list->var != NULL && list->var->name != NULL && list->var->name->len == name->len &&
            strncmp(list->var->name->str, name->str, name->len) == 0)
        {
            return list->var;
        }
        list = list->next;
    }
    return NULL;
}

int present_or_add(struct ListVariable **list, struct Variable *new_var)
{
    struct Variable *found_var = find_variable(*list, new_var->name);

    if (found_var != NULL)
    {
        struct Variable *supr = found_var;
        found_var = new_var;
        free_var(supr);
        return 1;
    }
    else
    {
        // Ajout de la nouvelle variable directement à la liste
        struct ListVariable *new_node = malloc(sizeof(struct ListVariable));
        if (new_node == NULL)
            return 0;
        new_node->var = new_var;
        new_node->next = NULL;
        if (*list == NULL)
        {
            *list = new_node;
        }
        else
        {
            struct ListVariable *temp = *list;
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = new_node;
        }
        return 1;
    }
}
