#include "merge_token.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
** copy la command dans cmd
** renvoi la cmd
*/
static struct Command *cpy_cmd(struct Command *cmd, struct Command *command)
{
    struct Token *cpy = NULL;
    cmd->tokenlist = malloc((command->nbtoken + 1) * sizeof(struct Command));
    while (cmd->nbtoken < command->nbtoken)
    {
        struct Token *token = command->tokenlist[cmd->nbtoken];
        cpy = malloc(sizeof(struct Token));
        cpy->len = token->len;
        cpy->quote = token->quote;
        cpy->str = malloc((cpy->len + 1) + sizeof(char));
        cpy->str = strncpy(cpy->str, token->str, cpy->len);
        cpy->str[cpy->len] = 0;
        cmd->tokenlist[cmd->nbtoken] = cpy;
        cmd->nbtoken++;
    }
    return cmd;
}

/*
** regarde si la variable a la place i de token existe
** si oui il renvoit la variable asociée
** sinon renvoi null
*/
static struct Variable *is_var(struct Token *token, struct ListVariable *ListVariable, size_t i)
{
    if (i >= token->len || token->str[i] != '$')
        return NULL;
    struct Token *tvar = malloc(sizeof(struct Token));
    tvar->str = token->str + i + 1;
    tvar->len = strcspn(tvar->str, " \n\r$\t;");
    struct Variable *var = find_variable(ListVariable, tvar);
    free(tvar);
    return var;
}

/*
** concatene la string de variable dans la token a la place i
*/
static void merge_token_var(struct Token *token, struct Variable *var, size_t i)
{
    size_t l = strlen(var->valeur);
    token->str = realloc(token->str, (token->len + l + 1) * sizeof(char));
    token->str[token->len + l] = 0;
    token->str[i] = 0;
    char *first = malloc(i + 1);
    first = strncpy(first, token->str, i);
    first[i] = 0;
    char *last = malloc(token->len - i);
    last = strcpy(last, token->str + i + var->name->len + 1);
    snprintf(token->str, token->len + l + 1, "%s%s%s", first, var->valeur, last);
    free(first);
    free(last);
    token->len = strlen(token->str);
}

/*
** modifie la token double quote pour ajouter les variable
** si une variable a été trouvé, retourne 1
** sinon retourn 0
*/
static void add_var_double_quote(struct Token *token, struct ListVariable *ListVariable)
{
    size_t i = 0;
    struct Variable *var = NULL;
    while (i < token->len)
    {
        var = is_var(token, ListVariable, i);
        if (var == NULL)
        {
            i++;
            continue;
        }
        merge_token_var(token, var, i);
        if (var->name->quote == SIMPLE_QUOTE)
            i += strlen(var->valeur);
    }
}

/*
** insert a la i eme place le token 
*/
static void insert_token_at_cmd(struct Command *cmd, struct Token *token, size_t i)
{
    size_t j = cmd->nbtoken;
    struct Token *aux = NULL;
    cmd->tokenlist = realloc(cmd->tokenlist, (j + 1) * sizeof(struct Token *));
    cmd->tokenlist[j] = token;
    while (j > i)
    {
        aux = cmd->tokenlist[j];
        cmd->tokenlist[j] = cmd->tokenlist[j - 1];
        cmd->tokenlist[j - 1] = aux;
        j--;
    }
    cmd->nbtoken++;
}

/*
** parse la variable et la rajoute a la cmd
*/
static void add_token_var(struct Command *cmd, struct Variable *var, size_t i)
{
    struct Token *token = NULL;
    size_t k = cmd->tokenlist[i]->len;
    size_t j = strcspn(var->valeur, " \n\r\t;");
    char *c = malloc((j + 1) * sizeof(char));
    c = strncpy(c, var->valeur, j);
    c[j] = 0;
    cmd->tokenlist[i]->str = realloc(cmd->tokenlist[i]->str, k + j + 1);
    cmd->tokenlist[i]->str = strcat(cmd->tokenlist[i]->str, c);
    cmd->tokenlist[i]->quote = var->name->quote;
    cmd->tokenlist[i]->len += j;
    free(c);
    i++;
    j += strspn(var->valeur + j, " \n\r\t;");
    while (var->valeur[j] != 0)
    {
        k = strcspn(var->valeur + j, " \n\r\t;");
        char *c = malloc((k + 1) * sizeof(char));
        c = strncpy(c, var->valeur + j, k);
        c[k] = 0;
        token = malloc(sizeof(struct Token));
        token->len = k;
        token->quote = var->name->quote;
        token->str = c;
        insert_token_at_cmd(cmd, token, i);
        i++;
        j += k;
        j += strspn(var->valeur + j, " \n\r\t;");
    }
}

/*
** ajoute les variable dans le cas ou on a pas de quote
** si une variable a été trouvé, retourne 1
** sinon retourn 0
*/
static int add_var_not_quote(struct Command *cmd, struct ListVariable *ListVariable, size_t i)
{
    struct Token *token = cmd->tokenlist[i];
    size_t rep = 0;
    size_t j = 0;
    struct Variable *var = NULL;
    while (j < token->len)
    {
        var = is_var(token, ListVariable, j);
        if (var == NULL)
        {
            j++;
            continue;
        }
        token->len = token->len - var->name->len - 1;
        token->str[token->len] = 0;
        add_token_var(cmd, var, i);
        if (token->quote == SIMPLE_QUOTE)
            break;
        rep = 1;
    }
    return rep;
}

/*
** modifie la command pour ajouter les variable au bonne endroi
** renvoie la command modifier
*/
static struct Command *add_var(struct Command *cmd, struct ListVariable *ListVariable)
{
    size_t i = 0;
    while (i < cmd->nbtoken)
    {
        if (cmd->tokenlist[i]->quote == SIMPLE_QUOTE)
            i++;
        else if (cmd->tokenlist[i]->quote == DOUBLE_QUOTE)
        {
            add_var_double_quote(cmd->tokenlist[i], ListVariable);
            i++;
        }
        else /// not quote
        {
            if (!add_var_not_quote(cmd, ListVariable, i))
                i++;
        }
    }
    return cmd;
}

struct Ast *cpy_cmd_with_var(struct Command *command, struct ListVariable *ListVariable)
{
    struct Command *cmd = malloc(sizeof(struct Command));
    cmd->nbtoken = 0;
    cmd->tokenlist = NULL;
    cmd = cpy_cmd(cmd, command);

    cmd = add_var(cmd, ListVariable);

    struct Ast *ast = calloc(1, sizeof(struct Ast));
    if (!ast)
        return NULL;
    ast->type = SIMPLE_CMD;
    ast->next = NULL;

    // Alloue la memoire pour la struct Ast_simple_cmd
    struct Ast_simple_cmd *sc = &ast->data.sc;

    sc->cmd = cmd;

    return ast;
}

void free_cmd_var(struct Ast *ast)
{
    // Alloue la memoire pour la struct Ast_simple_cmd
    struct Ast_simple_cmd *sc = &ast->data.sc;
    struct Command *command = sc->cmd;
    if (command == NULL)
        return;
    size_t i = 0;
    while (i < command->nbtoken)
    {
        free(command->tokenlist[i]->str);
        free(command->tokenlist[i]);
        i++;
    }
    free(command->tokenlist);
    free(command);
    free(ast);
}
