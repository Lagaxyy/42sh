#include "parsing.h"

#include "../lexer/lexer.h"

struct Command *init_command(void)
/*
** Renvoie une struct Command initialisée
*/
{
    struct Command *c = calloc(1,sizeof(struct Command));
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
        realloc(c->tokenlist, sizeof(struct Token) * (c->nbtoken + 1));
    c->tokenlist[c->nbtoken] = token;
    c->nbtoken++;
}

struct Ast *simple_command(struct Lexer *lex)
/*
** Renvoie un noeud de l'AST de type
** simple_command
**
** Grammaire:
**
** simple_command =
**      WORD { element } ;
**
** element = WORD ;
*/
{
    // Premier WORD necessaire
    struct Token *token = lexer(lex, WORD);
    if (!token)
        // Token incorrect
        return NULL;

    // Alloue la mémoire pour la struct Ast et définit le type
    struct Ast *ast = calloc(1, sizeof(struct Ast));
    if (!ast)
        return NULL;
    ast->type = SIMPLE_CMD;
    ast->next = NULL;

    // Alias pratique
    struct Ast_simple_cmd *sc = &ast->data.sc;

    // Initialise la struct Command et ajoute le premier token
    sc->cmd = init_command();
    ajout_command(sc->cmd, token);

    // Autres WORD
    token = lexer(lex, WORD);
    while (token != NULL)
    {
        ajout_command(sc->cmd, token);
        token = lexer(lex, WORD);
    }

    return ast;
}
