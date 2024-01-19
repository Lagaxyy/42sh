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

struct Ast *simple_command(struct Lexer *lex)
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
    struct Token *token = lexer(lex, WORD);
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

    // Autres WORD
    token = lexer(lex, WORD);
    while (token != NULL)
    {
        ajout_command(sc->cmd, token);
        token = lexer(lex, WORD);
    }

    return ast;
}

struct Ast *if_command(struct Lexer *lex)
/*
** Renvoie un noeud de l'AST de type
** if_command
**
** Grammaire:
**
** if_command =
**      'if' compound_list 'then' compound_list [ else_clause ] 'fi' ;
*/
{
    // si if pas trouver
    if (!lexerVerif(lex, IF_GRM))
    {
        if (!lexerVerif(lex, ELIF_GRM))
            // on continue
            return simple_command(lex);
    }

    // Alloue la memoire pour la struct Ast et definit le type
    struct Ast *ast = malloc(sizeof(struct Ast));
    if (!ast)
        return NULL;
    ast->type = IF_CMD;
    
    // Alloue la memoire pour la struct Ast_simple_cmd
    struct Ast_if *aif = &ast->data.aif;

    // trouver la condition
    aif->cdt = parse_start(lex);
    if (!aif->cdt)
    {
        free_ast(ast);
        return NULL;
    }

    // trouver le then
    if (!lexerVerif(lex, THEN_GRM))
    {
        free_ast(ast);
        return NULL;
    }
    aif->cmd1 = parse_start(lex);

    if (lexerVerif(lex, ELIF_GRM))
    {
        aif->cmd2 = if_command(lex);
    }
    else if (lexerVerif(lex, ELSE_GRM))
    {
        aif->cmd2 = parse_start(lex);
    }
    else
        aif->cmd2 = NULL;
    if (!lexerVerif(lex, FI_GRM))
    {
        free_ast(ast);
        return NULL;
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

struct Ast *parse_start(struct Lexer *lex)
{
    struct Ast *ast = if_command(lex);
    if (ast == NULL)
        return NULL;
    if (!lexerVerif(lex, SEMICOLON))
    {
        return ast;
    }
    return ast;
}

struct Ast *parse_command(struct Lexer *lexer)
{
    struct Ast *ast = parse_start(lexer);
    //regarder si sa finit par un ';'
    if (!ast)
        errx(1, "Erreur lors de la construction de l'AST");
    print_ast(ast, 0);
    return ast;
}
