#ifndef MERGE_TOKEN_H
#define MERGE_TOKEN_H

#include "../lexer/lexer.h"
#include "../parser/parsing.h"
#include "../variable/variable.h"
#include "exec_ast.h"

void merge_token(struct Command *command, struct ListVariable *ListVariable);

#endif /* ! MERGE_TOKEN_H */