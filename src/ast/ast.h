#ifndef AST_H
#define AST_H

enum ast_type
{
    IF,
    THEN,
    ELIF,
    ELSE,
    FI,
    SEMICOLON, /// ;
    LINE_BREAK, /// \n
    QUOT, /// '
    WORLD
};

struct ast
{
    enum ast_type type;
    char *value;
    struct ast *left;
    struct ast *right;
};

struct ast *ast_new(enum ast_type type);
void ast_free(struct ast *ast);

#endif /* ! AST_H */
