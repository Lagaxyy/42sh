#include "parsing.h"

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

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
        realloc(c->tokenlist, sizeof(struct Token) * (c->nbtoken + 1));
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
**      WORD { WORD }
*/
{
    // Premier WORD necessaire
    struct Token *token = lexer(str, WORD);
    if (!token)
        // Token incorrect
        return NULL;

    // Alloue la memoire pour la struct Ast et definit le type
    struct Ast *ast = malloc(sizeof(struct Ast));
    if (!ast)
        return NULL;
    ast->type = SIMPLE_CMD;

    // Alloue la memoire pour la struct Ast_simple_cmd
    struct Ast_simple_cmd *sc = &ast->data.sc; // Alias pratique

    // Initialise la struct Command et ajoute le premier token
    sc->cmd = init_command();
    ajout_command(sc->cmd, token);
    str = decale_str(token);

    // Autres WORD
    token = lexer(str, WORD);
    while (token != NULL)
    {
        ajout_command(sc->cmd, token);
        str = decale_str(token);
        token = lexer(str, WORD);
    }

    return ast;
}

void print_ast(struct Ast *ast, size_t depth)
{
    // Print le type du noeud actuel
    for (size_t i = 0; i < depth; i++)
        printf("  "); // Indente en fonction de la profondeur du noeud

    switch (ast->type)
    {
    case IF_CMD:
        printf("IF_CMD\n");
        break;
    case SIMPLE_CMD:
        printf("SIMPLE_CMD\n");
        break;
    default:
        printf("Type inconnu\n");
        break;
    }

    // Alias pratique
    struct Ast_simple_cmd *sc = &ast->data.sc;

    // Print la commande si elle existe
    if (ast->type == SIMPLE_CMD && sc->cmd != NULL)
    {
        for (size_t i = 0; i < depth + 1; i++)
            printf("  ");

        printf("Command:\n");

        for (size_t i = 0; i < sc->cmd->nbtoken; i++)
        {
            for (size_t j = 0; j < depth + 2; j++)
                printf("  ");
            int s = sc->cmd->tokenlist[i]->len;
            printf("Token %zu: %.*s\n", i + 1, s, sc->cmd->tokenlist[i]->str);
        }
    }
}

struct Ast *parse_command(char *str)
{
    struct Ast *ast = simple_command(str);
    if (!ast)
        errx(1, "Erreur lors de la construction de l'AST");

    return ast;
}
