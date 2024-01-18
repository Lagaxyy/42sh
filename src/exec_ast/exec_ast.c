#ifndef _POSIX_C_SOURCE
#    define _POSIX_C_SOURCE 200809L
#endif /* ! _POSIX_C_SOURCE */

#include "exec_ast.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

// Pointeur de fonction pour rediriger vers les autres
// fonctions selon le type du noeud
typedef void (*node_type)(struct Ast *ast);

void exec_ast_sc(struct Ast *ast)
/*
** Execute le noeud simple_command
*/
{
    if (!ast)
        return;
    assert(ast->type == SIMPLE_CMD);

    // Match la commande avec sa fonction correspondante
    struct Ast_simple_cmd *sc = &ast->data.sc; // Alias pratique
    char *cmd = strdup(sc->cmd->tokenlist[0]->str);
    cmd[sc->cmd->tokenlist[0]->len] = '\0';
    if (strcmp(cmd, "echo") == 0)
        // ECHO
        my_echo(ast);
    else
        run_cmds(ast);

    free(cmd);
}

void exec_ast_if(struct Ast *ast)
/*
** Execute le noeud if_command
*/
{
    if (!ast)
        return;
    assert(ast->type == IF_CMD);
    return;
}

void exec_ast(struct Ast *ast)
/*
** Execute l'AST
** Etape: simple_command
*/
{
    if (!ast)
        return;

    // Redirige vers les autres fonctions selon le type du noeud
    static const node_type functions[] = {
        [SIMPLE_CMD] = &exec_ast_sc, [IF_CMD] = &exec_ast_if
    };
    (*functions[ast->type])(ast);
}
