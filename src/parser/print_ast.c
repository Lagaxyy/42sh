#include "../parser/parsing.h"
#include <stdio.h>

void print_simple_cmd(struct Ast *ast, size_t depth)
{
    // Alias pratique
    struct Ast_simple_cmd *sc = &ast->data.sc;

    // Print la commande si elle existe
    if (ast->type == SIMPLE_CMD && sc->cmd != NULL)
    {
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
}

void print_ast(struct Ast *ast, size_t depth)
{
    if (!ast)
    {
        return;
    }

    for (size_t i = 0; i < depth; ++i)
    {
        putchar(' ');  // Indentation pour représenter la profondeur de l'arbre
    }

    switch (ast->type)
    {
        case IF_CMD:
            printf("IF\n");
            print_ast(ast->data.aif.cdt, depth + 2);
            printf("THEN\n");
            print_ast(ast->data.aif.cmd1, depth + 2);
            printf("ELSE\n");
            print_ast(ast->data.aif.cmd2, depth + 2);
            break;
        case SIMPLE_CMD:
            printf("SIMPLE COMMAND\n");
            print_simple_cmd(ast, depth);
            break;
        default:
            fprintf(stderr, "Type d'AST inconnu\n");
            break;
    }

    printf("\n");

    // Appel récursif pour imprimer les nœuds suivants dans la liste
    print_ast(ast->next, depth);
}
