#include <stdlib.h>

#include "parsing.h"

void free_ast_sc(struct Ast *ast)
/*
** Libère la mémoire allouée pour le
** noeud simple_command
*/
{
    if (!ast)
        return;

    // Alias pratique
    struct Ast_simple_cmd *sc = &ast->data.sc;

    // Libère la mémoire allouée pour la liste des tokens
    for (size_t i = 0; i < sc->cmd->nbtoken; i++)
        free(sc->cmd->tokenlist[i]);
    free(sc->cmd->tokenlist);

    // Libère la mémoire allouée pour le reste
    free(sc->cmd);
}

void free_ast_cl(struct Ast *ast)
/*
** Libère la mémoire allouée pour le
** noeud command_list
*/
{
    if (!ast)
        return;

    // Alias pratique
    struct Ast_cmd_list *cl = &ast->data.cl;

    // Libere la memoire allouee pour la liste des commandes
    for (size_t i = 0; i < cl->nbnodes; i++)
        free_ast(cl->list[i]);
    free(cl->list);
}

void free_ast_if(struct Ast *ast)
/*
** Libère la mémoire allouée pour le
** noeud simple_command
*/
{
    if (!ast)
        return;

    // Alias pratique
    struct Ast_if *aif = &ast->data.aif;

    // Libère la mémoire allouée pour chaque branche du noeud
    free_ast(aif->cdt);
    free_ast(aif->cmd1);
    free_ast(aif->cmd2);

    return;
}

void free_ast(struct Ast *ast)
/*
** Libère la mémoire allouée pour l'AST
*/
{
    if (!ast)
        return;

    // Redirige vers les autres fonctions selon le type du noeud
    switch (ast->type)
    {
    case SIMPLE_CMD:
        free_ast_sc(ast);
        break;
    case CMD_LIST:
        free_ast_cl(ast);
        break;
    case IF_CMD:
        free_ast_if(ast);
        break;
    default:
        break;
    }

    // Libère la mémoire allouée des autres commandes
    free_ast(ast->next);

    free(ast);
}
