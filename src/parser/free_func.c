#include <stdlib.h>

#include "parsing.h"

void free_ast_sc(struct Ast *ast)
/*
** Libere la memoire allouee pour le
** noeud simple_command
*/
{
    if (!ast)
        return;

    // Alias pratique
    struct Ast_simple_cmd *sc = &ast->data.sc;

    // Libere la memoire allouee pour la liste des tokens
    for (size_t i = 0; i < sc->cmd->nbtoken; i++)
        free(sc->cmd->tokenlist[i]);
    free(sc->cmd->tokenlist);

    // Libere la memoire allouee pour le reste
    free(sc->cmd);
}

void free_ast_if(struct Ast *ast)
/*
** Libere la memoire allouee pour le
** noeud simple_command
*/
{
    if (!ast)
        return;
    // Alias pratique

    struct Ast_if *aif = &ast->data.aif;

    // Libere la memoire allouee pour la liste des tokens
    free_ast(aif->cdt);
    free_ast(aif->cmd1);
    free_ast(aif->cmd2);

    return;
}

void free_ast(struct Ast *ast)
/*
** Libere la memoire allouee pour l'AST
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
    case IF_CMD:
        free_ast_if(ast);
        break;
    default:
        break;
    }

    free_ast(ast->next);

    free(ast);
}
