#ifndef MERGE_TOKEN_H
#define MERGE_TOKEN_H

#include "../lexer/lexer.h"
#include "../parser/parsing.h"
#include "../variable/variable.h"
#include "exec_ast.h"

struct Ast *cpy_cmd_with_var(struct Command *command, struct ListVariable *ListVariable);

void free_cmd_var(struct Ast *ast);

#endif /* ! MERGE_TOKEN_H */
