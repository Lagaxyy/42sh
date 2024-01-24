#include "parsing.h"

#include <err.h>
#include <stdbool.h>
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
    struct Ast *ast = calloc(1, sizeof(struct Ast));
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

/*struct Ast *liste_command(struct Lexer *lex)
{
    /
    ** Renvoie un noeud de l'AST de type
    ** simple_command
    **
    ** Grammaire:
    **
    ** liste_command =
    **      simple_command { ';' simple_command}
    /
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
}*/

struct Ast *elif_command(struct Lexer *lex)
{
    struct Ast *ast = calloc(1, sizeof(struct Ast));
    if (!ast)
        return NULL;
    ast->type = IF_CMD;
    ast->next = NULL;

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
        // on continue
        return simple_command(lex);
    }

    // Alloue la memoire pour la struct Ast et definit le type
    struct Ast *ast = calloc(1, sizeof(struct Ast));
    if (!ast)
        return NULL;
    ast->type = IF_CMD;
    ast->next = NULL;

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

struct Ast *until_cmd(struct Lexer *lex)
/*
** rule_until = 'until' compound_list 'do' compound_list 'done' ;
*/
{
    if (!lexerVerif(lex, UNTIL_GRM))
        return if_command(lex);
    struct Ast *ast = calloc(1, sizeof(struct Ast));
    if (!ast)
        return NULL;
    ast->type = UNTIL_CMD;
    ast->next = NULL;
    // Alloue la memoire pour la struct Ast_while_cmd
    struct Ast_until *ut = &ast->data.ut;
    ut->cdt = parse_start(lex);
    if (ut->cdt == NULL)
    {
        free(ast);
        return NULL;
    }
    // recupere do
    if (!lexerVerif(lex, DO_GRM))
    {
        free_ast(ut->cdt);
        free(ast);
        return NULL;
    }
    ut->do_cdt = parse_start(lex);
    if (ut->do_cdt == NULL)
    {
        free_ast(ut->cdt);
        free(ast);
        return NULL;
    }
    ///////////// recupere done
    if (!lexerVerif(lex, DONE_GRM))
    {
        free_ast(ast);
        return NULL;
    }
    return ast;
}

struct Ast *while_cmd(struct Lexer *lex)
/*
** rule_while = 'while' compound_list 'do' compound_list 'done' ;
*/
{
    if (!lexerVerif(lex, WHILE_GRM))
        return until_cmd(lex);
    struct Ast *ast = calloc(1, sizeof(struct Ast));
    if (!ast)
        return NULL;
    ast->type = WHILE_CMD;
    ast->next = NULL;
    // Alloue la memoire pour la struct Ast_while_cmd
    struct Ast_while *wh = &ast->data.wh;
    wh->cdt = parse_start(lex);
    if (wh->cdt == NULL)
    {
        free(ast);
        return NULL;
    }
    // recupere do
    if (!lexerVerif(lex, DO_GRM))
    {
        free_ast(wh->cdt);
        free(ast);
        return NULL;
    }
    wh->do_cdt = parse_start(lex);
    if (wh->do_cdt == NULL)
    {
        free_ast(wh->cdt);
        free(ast);
        return NULL;
    }
    ///////////// recupere done
    if (!lexerVerif(lex, DONE_GRM))
    {
        free_ast(ast);
        return NULL;
    }
    return ast;
}

struct Ast *pipelines_command(struct Lexer *lex)
/*
** Renvoie un noeud de l'AST de type
** pipelines_command
**
** Grammaire:
**
** pipe = [cmd] '|' [cmd];
*/
{
    struct Ast *ast = while_cmd(lex);
    if (ast == NULL)
        return NULL;
    if (lexerVerif(lex, PIPELINES))
    {
        // Alloue la memoire pour la struct Ast et definit le type
        struct Ast *ast_aux = calloc(1, sizeof(struct Ast));
        if (!ast_aux)
        {
            free_ast(ast);
            return NULL;
        }
        ast_aux->type = PIPELINE_CMD;
        ast_aux->next = NULL;

        // Alloue la memoire pour la struct Ast_simple_cmd
        struct Ast_pipelines *pl = &ast_aux->data.pl;

        pl->left = ast;
        pl->right = pipelines_command(lex);
        if (pl->right == NULL)
        {
            free_ast(ast_aux);
        }
        return ast_aux;
    }
    return ast;
}

// implémentation 'not' command '!'
struct Ast *neg_command(struct Lexer *lex)
{
    if (!lexerVerif(lex, NEG_GRM))
        return pipelines_command(lex);
    struct Ast *ast = calloc(1, sizeof(struct Ast));
    if (!ast)
    {
        free_ast(ast);
        return NULL;
    }
    ast->type = NEG_CMD;
    ast->next = NULL;

    // Alloue la memoire pour la struct Ast_simple_cmd
    struct Ast_neg *neg = &ast->data.neg;

    neg->prg = pipelines_command(lex);
    if (neg->prg == NULL)
    {
        free(ast);
    }
    return ast;
}

// implémentation de 'or' command '||'
struct Ast *or_command(struct Lexer *lex)
{
    struct Ast *ast = neg_command(lex);
    if (ast == NULL)
        return NULL;
    if (lexerVerif(lex, OR_GRM))
    {
        struct Ast *ast_aux = calloc(1, sizeof(struct Ast));
        if (!ast_aux)
        {
            free_ast(ast);
            return NULL;
        }
        ast_aux->type = OR_CMD;
        ast_aux->next = NULL;

        // Alloue la memoire pour la struct Ast_simple_cmd
        struct Ast_or *or = &ast_aux->data.or;

        or->left = ast;
        or->right = or_command(lex);
        if (or->right == NULL)
        {
            free_ast(ast_aux);
        }
        return ast_aux;
    }
    return ast;
}

// implémentation de 'and' command '&&'
struct Ast *and_command(struct Lexer *lex)
{
    struct Ast *ast = or_command(lex);
    if (ast == NULL)
        return NULL;
    if (lexerVerif(lex, AND_GRM))
    {
        struct Ast *ast_aux = calloc(1, sizeof(struct Ast));
        if (!ast_aux)
        {
            free_ast(ast);
            return NULL;
        }
        ast_aux->type = AND_CMD;
        ast_aux->next = NULL;

        // Alloue la memoire pour la struct Ast_simple_cmd
        struct Ast_and *and = &ast_aux->data.and;

        and->left = ast;
        and->right = and_command(lex);
        if (and->right == NULL)
        {
            free_ast(ast_aux);
        }
        return ast_aux;
    }
    return ast;
}

struct Ast *parse_start(struct Lexer *lex)
{
    struct Ast *ast = if_command(lex);

    if (ast == NULL)
        // Pas d'AST
        return NULL;
    if (!lexerVerif(lex, SEMICOLON))
        // Prochain token n'est pas un ';'
        return ast;
    if (!lexerVerif(lex, END) && lexer_check(lex) == false)
    {
        // Parse la prochaine commande
        struct Ast *next_ast = parse_start(lex);
        if (!next_ast)
        {
            // Prochaine commande vide
            free_ast(ast);
            return NULL;
        }
        ast->next = next_ast;
    }
    return ast;
}

struct Ast *parse_command(struct Lexer *lexer)
{
    if (lexer->input == NULL)
        errx(1, "Cannot open file t.txt");
    struct Ast *ast = parse_start(lexer);
    if (!ast)
        errx(1, "Erreur lors de la construction de l'AST");

    return ast;
}
