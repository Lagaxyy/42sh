#ifndef PARSING_H
#define PARSING_H

#include <stddef.h>
#include <stdlib.h>

/*
** Struct et enum concernant les tokens
*/
enum Grammaire
{
    END,
    WORD,
    SEMICOLON
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
    SIMPLE_CMD,
    CMD_LIST,
    IF_CMD
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

struct Ast_cmd_list
{
    struct Ast **list;
    size_t nbnodes;
};

union Ast_union
{
    struct Ast_simple_cmd sc;
    struct Ast_cmd_list cl;
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
struct Ast *simple_command(char *str);
struct Ast *command_list(char *str);
void print_ast(struct Ast *ast, size_t depth);
void free_ast(struct Ast *ast);

#endif /* ! PARSING_H */
