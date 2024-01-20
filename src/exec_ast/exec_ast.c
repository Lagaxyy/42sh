#ifndef _POSIX_C_SOURCE
#    define _POSIX_C_SOURCE 200809L
#endif /* ! _POSIX_C_SOURCE */

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

    // Match la commande avec sa fonction correspondante
    struct Ast_simple_cmd *sc = &ast->data.sc; // Alias pratique
    if (strncmp(sc->cmd->tokenlist[0]->str, "echo", 4) == 0)
        // ECHO
        my_echo(ast);
    else
        run_cmds(ast);

    return 1;
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

    if (exec_ast(aif->cdt))
    {
        return exec_ast(aif->cmd1);
    }
    else
    {
        return exec_ast(aif->cmd2);
    }
    
    /*// Alloue la memoire pour la struct Ast_simple_cmd
    struct Ast_if *aif = &ast->data.aif;*/

    return 1;;
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
        [SIMPLE_CMD] = &exec_ast_sc, [IF_CMD] = &exec_ast_if
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
