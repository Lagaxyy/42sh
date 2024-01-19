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
        return 0;
    assert(ast->type == SIMPLE_CMD);

    // Match la commande avec sa fonction correspondante
    struct Ast_simple_cmd *sc = &ast->data.sc; // Alias pratique
    char *cmd = strdup(sc->cmd->tokenlist[0]->str);
    cmd[sc->cmd->tokenlist[0]->len] = '\0';
    int exitcode = 0;
    if (strcmp(cmd, "echo") == 0)
        // ECHO
        exitcode = my_echo(ast);
    else if (strcmp(cmd, "true") == 0)
        // TRUE
        exitcode = 0;
    else if (strcmp(cmd, "false") == 0)
        // FALSE
        exitcode = 1;
    else
        exitcode = run_cmds(ast);

    free(cmd);

    return exitcode;
}

int exec_ast_cl(struct Ast *ast)
/*
** Execute le noeud command_list
*/
{
    if (!ast)
        return 0;
    assert(ast->type == CMD_LIST);

    struct Ast_cmd_list *cl = &ast->data.cl; // Alias pratique
    int exitcode = 0;

    for (size_t i = 0; i < cl->nbnodes; i++)
        exitcode = exec_ast(cl->list[i]);

    return exitcode;
}

int exec_ast_if(struct Ast *ast)
/*
** Execute le noeud if_command
*/
{
    if (!ast)
        return 0;
    assert(ast->type == IF_CMD);
    return 0;
}

int exec_ast(struct Ast *ast)
/*
** Execute l'AST
** Etape: simple_command
*/
{
    if (!ast)
        return 0;

    // Redirige vers les autres fonctions selon le type du noeud
    static const node_type functions[] = {
        [SIMPLE_CMD] = &exec_ast_sc, [IF_CMD] = &exec_ast_if,
        [CMD_LIST] = &exec_ast_cl
    };
    return (*functions[ast->type])(ast);
}
