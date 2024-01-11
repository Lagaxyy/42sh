#include "ast.h"

#include <stdlib.h>

struct ast *ast_new(enum ast_type type)
{
    struct ast *new = calloc(1, sizeof(struct ast));
    if (!new)
        return NULL;
    new->type = type;
    return new;
}

void ast_free(struct ast *ast)
{
    if (ast == NULL)
        return;
    for (size_t i = 0; i < ast->nbchild; i++)
    {
        ast_free(ast->child[i]);
    }
    // apeler la fonction free token
    free(ast->value);
    free(ast->child);
    free(ast->next);
    free(ast);
}
