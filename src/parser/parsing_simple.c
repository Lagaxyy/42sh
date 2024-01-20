#include "parsing.h"

#include <err.h>
#include <stdio.h>
#include <string.h>

#include "../lexer/lexer.h"

struct Command *init_command(void)
/*
** Renvoie une struct Command initialisee
*/
{
    struct Command *c = malloc(sizeof(struct Command));
    c->tokenlist = NULL;
    c->nbtoken = 0;
    return c;
}

void ajout_command(struct Command *c, struct Token *token)
/*
** Ajoute un WORD a la struct Command
*/
{
    c->tokenlist =
        realloc(c->tokenlist, sizeof(struct Token *) * (c->nbtoken + 1));
    c->tokenlist[c->nbtoken] = token;
    c->nbtoken++;
}

char *decale_str(struct Token *token)
/*
** Renvoie un pointeur sur l'input
** mis a jour en fonction du token
*/
{
    return token->str + token->len;
}

struct Ast *simple_command(char *str)
/*
** Renvoie un noeud de l'AST de type
** simple_command
**
** Grammaire:
**
** simple_command =
**      WORD { WORD } ;
*/
{
    int end_cmd = 0;

    // Premier WORD necessaire
    struct Token *token = lexer(str, &end_cmd, WORD);
    if (!token)
        // Token incorrect
        return NULL;

    // Alloue la memoire pour la struct Ast et definit le type
    struct Ast *ast = malloc(sizeof(struct Ast));
    if (!ast)
        return NULL;
    ast->type = SIMPLE_CMD;

    struct Ast_simple_cmd *sc = &ast->data.sc; // Alias pratique

    // Initialise la struct Command et ajoute le premier token
    sc->cmd = init_command();
    ajout_command(sc->cmd, token);
    str = decale_str(token);

    // Autres WORD
    token = lexer(str, &end_cmd, WORD);
    while (!end_cmd)
    {
        ajout_command(sc->cmd, token);
        str = decale_str(token);
        token = lexer(str, &end_cmd, WORD);
    }
    if (token != NULL)
        // Commande terminee par un ';'
        ajout_command(sc->cmd, token);

    return ast;
}
