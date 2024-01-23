#ifndef _POSIX_C_SOURCE
#    define _POSIX_C_SOURCE 200809L
#endif /* ! _POSIX_C_SOURCE */

#include "../mypipe/mypipe.h"
#include "exec_ast.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

// Pointeur de fonction pour rediriger vers les autres
// fonctions selon le type du noeud
typedef int (*node_type)(struct Ast *ast);

int exec_ast_sc(struct Ast *ast)
/*
** Execute le noeud simple_command
*/
{
    if (!ast)
        return 1;
    assert(ast->type == SIMPLE_CMD);

    int rep = 1;

    // Match la commande avec sa fonction correspondante
    struct Ast_simple_cmd *sc = &ast->data.sc; // Alias pratique
    if (strncmp(sc->cmd->tokenlist[0]->str, "echo", 4) == 0)
        // ECHO
        rep = my_echo(ast);
    else if (strncmp(sc->cmd->tokenlist[0]->str, "true", 4) == 0)
        rep = 0;
    else if (strncmp(sc->cmd->tokenlist[0]->str, "false", 4) == 0)
        rep = 1;
    else
        rep = run_cmds(ast);

    return rep;
}

int exec_ast_if(struct Ast *ast)
/*
** Execute le noeud if_command
*/
{
    if (!ast)
        return 1;
    assert(ast->type == IF_CMD);

    struct Ast_if *aif = &ast->data.aif;

    if (!exec_ast(aif->cdt))
    {
        return exec_ast(aif->cmd1);
    }
    else
    {
        return exec_ast(aif->cmd2);
    }

    /*// Alloue la memoire pour la struct Ast_simple_cmd
    struct Ast_if *aif = &ast->data.aif;*/

    return 1;
    ;
}

int exec_ast_pipe(struct Ast *ast)
{
    if (!ast)
        return 1;
    assert(ast->type == PIPELINE_CMD);

    struct Ast_pipelines *pl = &ast->data.pl;

    return exec_pipe(pl->left, pl->right);
}

int exec_ast_until(struct Ast *ast)
{
    if (!ast)
        return 1;
    assert(ast->type == UNTIL_CMD);

    struct Ast_until *ut = &ast->data.ut;
    int res = 0;
    // si s'est 0 continue
    while (exec_ast(ut->cdt) != 0)
    {
        res = exec_ast(ut->do_cdt);
    }
    return res;
}

int exec_ast_while(struct Ast *ast)
{
    if (!ast)
        return 1;
    assert(ast->type == WHILE_CMD);

    struct Ast_while *wh = &ast->data.wh;
    int res = 0;
    // si s'est 0 continue
    while (exec_ast(wh->cdt) == 0)
    {
        res = exec_ast(wh->do_cdt);
    }
    return res;
}

int exec_ast_and(struct Ast *ast)
{
    if (!ast)
        return 1;
    assert(ast->type == AND_CMD);

    struct Ast_and *and = &ast->data.and;

    int res = exec_ast(and->left);
    if (res != 0)
        return res;
    return exec_ast(and->right);
}

int exec_ast_or(struct Ast *ast)
{
    if (!ast)
        return 1;
    assert(ast->type == OR_CMD);

    struct Ast_or *or = &ast->data.or;

    int res = exec_ast(or->left);
    if (res != 0)
        return exec_ast(or->right);
    return res;
}

int exec_ast_neg(struct Ast *ast)
{
    if (!ast)
        return 1;
    assert(ast->type == NEG_CMD);

    struct Ast_neg *neg = &ast->data.neg;

    return ! exec_ast(neg->prg);
}

int exec_ast(struct Ast *ast)
/*
** Execute l'AST
** Etape: simple_command
*/
{
    if (!ast)
        return 1;

    // Redirige vers les autres fonctions selon le type du noeud
    static const node_type functions[] = {
        [SIMPLE_CMD] = &exec_ast_sc, [IF_CMD] = &exec_ast_if, [PIPELINE_CMD] = &exec_ast_pipe,
        [WHILE_CMD] = &exec_ast_while, [UNTIL_CMD] = &exec_ast_until, [AND_CMD] = &exec_ast_and,
        [OR_CMD] = &exec_ast_or, [NEG_CMD] = &exec_ast_neg
    };

    int i = (*functions[ast->type])(ast);
    if (ast->next == NULL)
    {
        return i;
    }
    else
    {
        return exec_ast(ast->next);
    }
}
