#include "parsing.h"

#include <err.h>
#include <stdio.h>
#include <string.h>

#include "../lexer/lexer.h"

struct Ast *parse_command(char *str)
{
    if (strlen(str) == 0)
        // Empty
        return NULL;

    struct Ast *ast = command_list(str);
    if (!ast)
        errx(1, "Erreur lors de la construction de l'AST");

    return ast;
}
