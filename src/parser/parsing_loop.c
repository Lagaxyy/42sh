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
    wh->cdt = lexer(lex);
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
    wh->do_cdt = simple_command(lex);
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

struct Ast *for_cmd(struct Lexer *lex)
/*
** Renvoie un noeud de l'AST de type
** for
**
** Grammaire:
**
** rule_for = 'for' identifier 'in' wordlist 'do' compound_list 'done' ;
*/
{
    // Pas de 'for' trouvé
    if (!lexerVerif(lex, FOR_GRM))
        return while_cmd(lex);

    // Alloue la mémoire pour la struct Ast et définit le type
    struct Ast *ast = calloc(1, sizeof(struct Ast));
    if (!ast)
        return NULL;
    ast->type = FOR_CMD;
    ast->next = NULL;

    // Alias pratique
    struct Ast_for *fr = &ast->data.fr;

    // Parse l'identifiant
    fr->var = lexer(lex);
    if (fr->var == NULL)
    {
        free(ast);
        return NULL;
    }

    // Récupère le 'in'
    if (!lexerVerif(lex, IN_GRM))
    {
        free_ast(fr->var);
        free(ast);
        return NULL;
    }

    // Parse la liste de mots (wordlist)
    fr->valeurs = simple_command(lex);
    if (fr->valeurs == NULL)
    {
        free_ast(fr->var);
        free(ast);
        return NULL;
    }

    // Récupère le 'do'
    if (!lexerVerif(lex, DO_GRM))
    {
        free_ast(fr->valeurs);
        free_ast(fr->var);
        free(ast);
        return NULL;
    }

    // Parse la liste de commandes à exécuter dans la boucle
    fr->do_cdt = simple_command(lex);
    if (fr->do_cdt == NULL)
    {
        free_ast(fr->valeurs);
        free_ast(fr->var);
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