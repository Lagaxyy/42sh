#include "parsing.h"

#include <err.h>
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
    ast->next = NULL;

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

struct Ast *liste_command(struct Lexer *lex)
{
    /*
    ** Renvoie un noeud de l'AST de type
    ** simple_command
    **
    ** Grammaire:
    **
    ** liste_command =
    **      simple_command { ';' simple_command}
    */
    // simple_command
    struct Ast *ast = simple_command(lex);
    if (ast == NULL || ast->type != SIMPLE_CMD)
    {
        return NULL;
    }

    // { ';' simple_command}
    struct Ast *p = ast;

    while (1)
    {
        int s = lex->pos;
        if (lexerVerif(lex, SEMICOLON) == 0 || lexer_chek(lex) == true)
        {
            lex->pos = s;
            return ast;
        }
        p->next = simple_command(lex);
        p = p->next;
        if (p == NULL)
        {
            lex->pos = s;
            return ast;
        }
    }
    return ast;
}

struct Ast *elif_command(struct Lexer *lex)
{
    struct Ast *ast = calloc(1, sizeof(struct Ast));
    if (!ast)
        return NULL;
    ast->type = IF_CMD;

    struct Ast_if *aif = &ast->data.aif;

    aif->cdt = parse_start(lex);

    if (!aif->cdt)
    {
        free_ast(ast);
        return NULL;
    }
    if (!lexerVerif(lex, THEN_GRM))
    {
        free_ast(ast);
        return NULL;
    }
    aif->cmd1 = parse_start(lex);

    if (lexerVerif(lex, ELIF_GRM))
    {
        aif->cmd2 = elif_command(lex);
    }
    else if (lexerVerif(lex, ELSE_GRM))
    {
        aif->cmd2 = parse_start(lex);
    }
    else
        aif->cmd2 = NULL;

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
    struct Ast *ast = calloc(1, sizeof(struct Ast));
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
        aif->cmd2 = elif_command(lex);
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

struct Ast *parse_start(struct Lexer *lex)
{
    struct Ast *ast = if_command(lex);
    if (ast == NULL)
        return NULL;
    if (!lexerVerif(lex, SEMICOLON))
        return ast;
    if (!lexerVerif(lex, END) && lexer_chek(lex) == false)
    {
        struct Ast *next_ast = parse_start(lex);
        if (!next_ast)
        {
            free_ast(ast);
            return NULL;
        }
        ast->next = next_ast;
    }
    return ast;
}

struct Ast *parse_command(struct Lexer *lexer)
{
    struct Ast *ast = parse_start(lexer);
    if (!ast)
        errx(2, "Erreur lors de la construction de l'AST");

    return ast;
}
