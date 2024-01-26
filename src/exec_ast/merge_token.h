#ifndef MERGE_TOKEN_H
#define MERGE_TOKEN_H

#include "../parser/parsing.h"
#include "../lexer/lexer.h"
#include "exec_ast.h"
#include "../variable/variable.h"

void merge_token(struct Command *command, struct ListVariable *ListVariable);

#endif /* ! MERGE_TOKEN_H */