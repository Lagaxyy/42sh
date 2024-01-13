#ifndef PARSING_H
#define PARSING_H

#include <stddef.h>

enum Ast_type
{
    IF,
    THEN,
    ELIF,
    ELSE,
    FI,
    COMAND
};

enum Grammaire
{
    END,
    WORD
};

struct Token
{
    char *str;
    size_t len;
};


struct Command
{
    struct Token **token;
    size_t nbtoken;
};

struct Ast
{
    enum Ast_type type;
    struct Command *command;
    size_t nbchild;
    struct Ast **child;
    struct Ast *next;
};

//struct ast *ast_new(enum ast_type type);
//void ast_free(struct ast *ast);
void parse_command(char *str);

#endif /* ! PARSING_H */
