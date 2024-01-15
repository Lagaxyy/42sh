#ifndef EXEC_AST_H
#define EXEC_AST_H

#include "../parser/parsing.h"

void exec_ast(struct Ast *ast);
int my_echo(struct Ast *ast);

#endif /* ! EXEC_AST_H */
