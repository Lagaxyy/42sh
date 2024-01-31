#include "parsing.h"

#include "../lexer/lexer.h"

struct Ast *until_cmd(struct Lexer *lex)
/*
** Renvoie un noeud de l'AST de type
** until
**
** Grammaire:
**
** rule_until = 'until' compound_list 'do' compound_list 'done' ;
*/
{
    // Pas de 'until' trouvé
    if (!lexerVerif(lex, UNTIL_GRM))
        return if_command(lex);

    // Alloue la mémoire pour la struct Ast et définit le type
    struct Ast *ast = calloc(1, sizeof(struct Ast));
    if (!ast)
        return NULL;
    ast->type = UNTIL_CMD;
    ast->next = NULL;

    // Alias pratique
    struct Ast_until *ut = &ast->data.ut;

    // Parse la commande
    ut->cdt = parse_start(lex);
    if (ut->cdt == NULL)
    {
        free(ast);
        return NULL;
    }
    // Récupère le 'do'
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
    // Récupère le 'done'
    if (!lexerVerif(lex, DONE_GRM))
    {
        free_ast(ast);
        return NULL;
    }
    return ast;
}

struct Ast *while_cmd(struct Lexer *lex)
/*
** Renvoie un noeud de l'AST de type
** while
**
** Grammaire:
**
** rule_while = 'while' compound_list 'do' compound_list 'done' ;
*/
{
    // Pas de 'while' trouvé
    if (!lexerVerif(lex, WHILE_GRM))
        return until_cmd(lex);

    // Alloue la mémoire pour la struct Ast et définit le type
    struct Ast *ast = calloc(1, sizeof(struct Ast));
    if (!ast)
        return NULL;
    ast->type = WHILE_CMD;
    ast->next = NULL;

    // Alias pratique
    struct Ast_while *wh = &ast->data.wh;

    // Parse la commande
    wh->cdt = parse_start(lex);
    if (wh->cdt == NULL)
    {
        free(ast);
        return NULL;
    }
    // Récupère le 'do'
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
    // Récupère le 'done'
    if (!lexerVerif(lex, DONE_GRM))
    {
        free_ast(ast);
        return NULL;
    }
    return ast;
}
