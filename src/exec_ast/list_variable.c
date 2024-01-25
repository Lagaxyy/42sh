#include "../variable/variable.h"

#include "exec_ast.h"

struct Variable *create_variable(struct Token *name, const char *value)
{
    struct Variable *new_var = malloc(sizeof(struct Variable));
    if (new_var != NULL)
    {
        new_var->name = name;
        new_var->valeur = strdup(value);
    }
    return new_var;
}

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

void present_or_add(struct ListVariable **list, struct Variable *new_var)
{
    struct Variable *found_var = find_variable(*list, new_var->name);

    if (found_var != NULL)
    {
        free(found_var->valeur);
        found_var->valeur = new_var->valeur;
        return found_var->valeur;//changer la valeur
    }
    else
    {
        // Ajout de la nouvelle variable directement à la liste
        struct ListVariable *new_node = malloc(sizeof(struct ListVariable));
        if (new_node == NULL)
            return NULL;
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
        return new_var->valeur;
    }
}
