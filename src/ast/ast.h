#ifndef AST_H
#define AST_H

#include <stddef.h>

enum ast_type
{
    IF,
    THEN,
    ELIF,
    ELSE,
    FI,
    COMAND
};

struct ast
{
    enum ast_type type;
    char *value;
    size_t nbchild;
    struct ast **child;
    struct ast *next;
    //struct token *token;
};

struct ast *ast_new(enum ast_type type);
void ast_free(struct ast *ast);

#endif /* ! AST_H */
