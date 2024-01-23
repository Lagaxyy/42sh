#ifndef EXEC_AST_H
#define EXEC_AST_H

#include "../parser/parsing.h"

int exec_ast(struct Ast *ast);
int my_echo(struct Ast *ast);
int run_cmds(struct Ast *ast);

#endif /* ! EXEC_AST_H */
