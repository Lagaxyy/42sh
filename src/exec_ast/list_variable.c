#include "exec_ast.h"

struct Token {
    char *str;
    size_t len;
};

struct Variable {
    struct Token *name;
    char *value;
};

struct ListVariable {
    struct Variable *var;
    struct ListVariable *next;
};

struct Variable *create_variable(struct Token *name, const char *value) {
    struct Variable *new_var = (struct Variable *)malloc(sizeof(struct Variable));
    if (new_var != NULL) {
        new_var->name = name;
        new_var->value = strdup(value);
    }
    return new_var;
}

void free_variable(struct Variable *var) {
    if (var != NULL) {
        free(var->value);
        free(var->name->str);  // Libération de la mémoire allouée pour le str dans Token
        free(var->name);       // Libération de la mémoire allouée pour la structure Token
        free(var);
    }
}

void free_list(struct ListVariable *list) {
    while (list != NULL) {
        struct ListVariable *temp = list;
        list = list->next;
        free_variable(temp->var);
        free(temp);
    }
}

struct Variable *find_variable(struct ListVariable *list, struct Token *name) {
    while (list != NULL) {
        if (list->var != NULL && list->var->name != NULL && list->var->name->len == name->len &&
            strcmp(list->var->name->str, name->str) == 0) {
            return list->var;
        }
        list = list->next;
    }
    return NULL;
}

void add_variable(struct ListVariable **list, struct Variable *var) {
    struct ListVariable *new_node = (struct ListVariable *)malloc(sizeof(struct ListVariable));
    if (new_node != NULL) {
        new_node->var = var;
        new_node->next = NULL;

        if (*list == NULL) {
            *list = new_node;
        } else {
            struct ListVariable *temp = *list;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = new_node;
        }
    }
}

char *present_or_add(struct ListVariable **list, struct Variable *new_var)
{
    struct Variable *found_var = find_variable(*list, new_var->name);

    if (found_var != NULL) {
        return found_var->value;
    } else {
        // Ajout de la nouvelle variable directement à la liste
        struct ListVariable *new_node = (struct ListVariable *)malloc(sizeof(struct ListVariable));
        if (new_node != NULL) {
            new_node->var = new_var;
            new_node->next = NULL;

            if (*list == NULL) {
                *list = new_node;
            } else {
                struct ListVariable *temp = *list;
                while (temp->next != NULL) {
                    temp = temp->next;
                }
                temp->next = new_node;
            }

            return new_var->value;
        } else {
            return NULL;
        }
    }
}

int main() {
    // Exemple d'utilisation
    struct ListVariable *variable_list = NULL;

    // Création de quelques tokens et variables
    struct Token token1 = { "Var1", 4 };
    struct Token token2 = { "Var2", 4 };

    struct Variable var1 = { &token1, "Value1" };
    struct Variable var2 = { &token2, "Value2" };

    // Utilisation de la fonction present_or_add
    char *value1 = present_or_add(&variable_list, &var1);
    char *value2 = present_or_add(&variable_list, &var2);

    // Affichage des valeurs des variables
    printf("Value of Var1: %s\n", value1);
    printf("Value of Var2: %s\n", value2);

    // Libération de la mémoire à la fin du programme
    free_list(variable_list);

    return 0;
}