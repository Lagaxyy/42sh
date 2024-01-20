#include "parsing.h"

#include <err.h>
#include <stdio.h>
#include <string.h>

#include "../lexer/lexer.h"

void print_ast(struct Ast *ast, size_t depth)
{
    if (!ast)
        return;

    // Print le type du noeud actuel
    for (size_t i = 0; i < depth; i++)
        printf("  "); // Indente en fonction de la profondeur du noeud

    switch (ast->type)
    {
    case SIMPLE_CMD:
        printf("SIMPLE_CMD\n");
        break;
    case CMD_LIST:
        printf("CMD_LIST\n");
        break;
    case IF_CMD:
        printf("IF_CMD\n");
        break;
    default:
        printf("Type inconnu\n");
        break;
    }

    // Print la commande si elle existe
    if (ast->type == SIMPLE_CMD)
    {
        // Alias pratique
        struct Ast_simple_cmd *sc = &ast->data.sc;

        for (size_t i = 0; i < depth + 1; i++)
            printf("  ");

        printf("Command:\n");

        for (size_t i = 0; i < sc->cmd->nbtoken; i++)
        {
            for (size_t j = 0; j < depth + 2; j++)
                printf("  ");
            int s = sc->cmd->tokenlist[i]->len;
            printf("Token %zu: %.*s\n", i + 1, s, sc->cmd->tokenlist[i]->str);
        }
    }
    else if (ast->type == CMD_LIST)
    {
        struct Ast_cmd_list *cl = &ast->data.cl; // Alias pratique

        for (size_t i = 0; i < depth + 1; i++)
            printf("  ");

        printf("List:\n");

        for (size_t i = 0; i < cl->nbnodes; i++)
            print_ast(cl->list[i], depth + 1);
    }
}
