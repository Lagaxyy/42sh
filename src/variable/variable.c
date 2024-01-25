#include "../lexer/lexer.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "variable.h"

bool lettre(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool alphanumerique(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

/*bool verif_nom_variable_ancien(struct Token *token)
{
    if (token->str == 0 || lettre(*token->str) == false)
    {
        return false;
    }
    size_t i = 0;
    while (i < token->len)
    {
        if (alphanumerique(token->str[i]) == false)
        {
            return false;
        }
        i++;
    }
    return true;
}*/

bool verif_nom_variable(struct Token *token)
{
    // du type (NOM_DE_LA_VARIABLE)*$
    if (token->str == 0 || lettre(*token->str) == false)
    {
        return false;
    }
    size_t i = 1;
    while (i < token->len && token->str[i] != '=')
    {
        if (alphanumerique(token->str[i]) == false)
        {
            return false;
        }
        i++;
    }
    if (token->str[i] != '=')//i != token->len
    {
        return false;
    }
    return true;
}

bool verif_nom_variable2(struct Token *token)
{
    // du type (NOM_DE_LA_VARIABLE)=$
    if (token->str == 0 || lettre(*token->str) == false)
    {
        return false;
    }
    size_t i = 1;
    while (i < token->len && token->str[i] != '=')
    {
        if (alphanumerique(token->str[i]) == false)
        {
            return false;
        }
        i++;
    }
    if (token->str[i] != '=')//i != token->len
    {
        return false;
    }
    
    return true;
}

bool is_variable(struct Command *command)
/*
** verifi si le nom de variable est bon
*/
{
    if (command->nbtoken == 2)
    {
        if (verif_nom_variable2(command->tokenlist[0]) == false)
        {
            return false;
        }
        return true;
    }
    
    if (command->nbtoken != 1 || verif_nom_variable(command->tokenlist[0]) == false)
    {
        return false;
    }
    return true;
}


struct Variable *varialble_parsing(struct Command *command)
{
    if (is_variable(command) == false)
    {
        return NULL;
    }
    struct Variable *variable = malloc(sizeof(struct Variable));
    variable->name = malloc(sizeof(struct Token));
    size_t t = 0;
    if (command->nbtoken == 2)
    {
        variable->name->str = command->tokenlist[0]->str;
        variable->name->len = command->tokenlist[0]->len - 1;

        t = command->tokenlist[1]->len;
        variable->valeur = malloc(sizeof(char) * (t + 1));
        size_t i = 0;
        while (i < t)
        {
            variable->valeur[i] = command->tokenlist[1]->str[i];
            i++;
        }
        variable->valeur[i] = 0;
    }
    else
    {
        size_t pos_egal = 0;
        struct Token *token = command->tokenlist[0]; // alias
        while (token->str[pos_egal] != '=')
        {
            pos_egal++;
        }

        variable->name->str = command->tokenlist[0]->str;
        variable->name->len = pos_egal;

        t = command->tokenlist[0]->len - pos_egal;
        variable->valeur = malloc(sizeof(char) * (t + 1));
        size_t i = 0;
        while (i < t)
        {
            variable->valeur[i] = token->str[i + pos_egal + 1];
            i++;
        }
        variable->valeur[i] = 0;
    }
    
    return variable;
}

void free_var(struct Variable *var)
{
    if (var == NULL)
        return;
    free(var->name);
    free(var->valeur);
    free(var);
}
