#ifndef PARSING_H
#define PARSING_H

#include <stddef.h>

/*
** Struct et enum concernant les tokens
*/
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

/*
** Structs contenant les datas
*/
struct Command
{
    struct Token **tokenlist;
    size_t nbtoken;
};

/*
** Structs et enum definissant l'AST
*/
enum Ast_type
{
    IF_CMD,
    SIMPLE_CMD
};

// (TODO) A completer
struct Ast_if
{
    void *rien;
};

struct Ast_simple_cmd
{
    struct Command *cmd;
};

union Ast_union
{
    struct Ast_simple_cmd sc;
    struct Ast_if aif;
};

struct Ast
{
    enum Ast_type type;
    union Ast_union data;
};

/*
** Fonctions exportees
*/
struct Ast *parse_command(char *str);
void print_ast(struct Ast *ast, size_t depth);
void free_ast(struct Ast *ast);

#endif /* ! PARSING_H */
