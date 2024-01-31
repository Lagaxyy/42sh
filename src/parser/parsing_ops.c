#include "parsing.h"

#include "../lexer/lexer.h"

struct Ast *pipelines_command(struct Lexer *lex)
/*
** Renvoie un noeud de l'AST de type
** pipeline
**
** Grammaire:
**
** pipeline = command { '|' {'\n'} command } ;
*/
{
    // Parse la commande
    struct Ast *ast = while_cmd(lex);
    if (ast == NULL)
        return NULL;

    // Si '|' trouvé
    if (lexerVerif(lex, PIPELINES))
    {
        // Alloue la mémoire pour la struct Ast et définit le type
        struct Ast *ast_aux = calloc(1, sizeof(struct Ast));
        if (!ast_aux)
        {
            free_ast(ast);
            return NULL;
        }
        ast_aux->type = PIPELINE_CMD;
        ast_aux->next = NULL;

        // Alias pratique
        struct Ast_pipelines *pl = &ast_aux->data.pl;

        // Parse la commande
        pl->left = ast;
        pl->right = pipelines_command(lex);
        if (pl->right == NULL)
        {
            free_ast(ast_aux);
            return NULL;
        }
        return ast_aux;
    }
    return ast;
}

struct Ast *neg_command(struct Lexer *lex)
/*
** Renvoie un noeud de l'AST de type
** negation
**
** Grammaire:
**
** pipeline = ['!'] command { '|' {'\n'} command } ;
*/
{
    // Pas de '!' trouvé
    if (!lexerVerif(lex, NEG_GRM))
        return pipelines_command(lex);

    // Alloue la mémoire pour la struct Ast et définit le type
    struct Ast *ast = calloc(1, sizeof(struct Ast));
    if (!ast)
    {
        free_ast(ast);
        return NULL;
    }
    ast->type = NEG_CMD;
    ast->next = NULL;

    // Alias pratique
    struct Ast_neg *neg = &ast->data.neg;

    // Parse la commande
    neg->prg = pipelines_command(lex);
    if (neg->prg == NULL)
    {
        free(ast);
        return NULL;
    }
    return ast;
}

struct Ast *or_command(struct Lexer *lex)
/*
** Renvoie un noeud de l'AST de type
** or
**
** Grammaire:
**
** and_or = pipeline { ( '&&' | '||' ) {'\n'} pipeline } ;
*/
{
    // Parse la commande
    struct Ast *ast = neg_command(lex);
    if (ast == NULL)
        return NULL;

    // Si '||' trouvé
    if (lexerVerif(lex, OR_GRM))
    {
        // Alloue la mémoire pour la struct Ast et définit le type
        struct Ast *ast_aux = calloc(1, sizeof(struct Ast));
        if (!ast_aux)
        {
            free_ast(ast);
            return NULL;
        }
        ast_aux->type = OR_CMD;
        ast_aux->next = NULL;

        // Alias pratique
        struct Ast_or *or = &ast_aux->data.or;

        // Parse la commande
        or->left = ast;
        or->right = or_command(lex);
        if (or->right == NULL)
        {
            free_ast(ast_aux);
            return NULL;
        }
        return ast_aux;
    }
    return ast;
}

struct Ast *and_command(struct Lexer *lex)
/*
** Renvoie un noeud de l'AST de type
** and
**
** Grammaire:
**
** and_or = pipeline { ( '&&' | '||' ) {'\n'} pipeline } ;
*/
{
    // Parse la commande
    struct Ast *ast = or_command(lex);
    if (ast == NULL)
        return NULL;

    // Si '&&' trouvé
    if (lexerVerif(lex, AND_GRM))
    {
        // Alloue la mémoire pour la struct Ast et définit le type
        struct Ast *ast_aux = calloc(1, sizeof(struct Ast));
        if (!ast_aux)
        {
            free_ast(ast);
            return NULL;
        }
        ast_aux->type = AND_CMD;
        ast_aux->next = NULL;

        // Alias pratique
        struct Ast_and *and = &ast_aux->data.and;

        // Parse la commande
        and->left = ast;
        and->right = and_command(lex);
        if (and->right == NULL)
        {
            free_ast(ast_aux);
            return NULL;
        }
        return ast_aux;
    }
    return ast;
}
