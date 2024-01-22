#ifndef PARSING_H
#define PARSING_H

#include <stddef.h>
#include <stdlib.h>

struct Ast;
struct Lexer;

/*
** Struct et enum concernant les tokens
*/
enum Grammaire
{
    WORD,
    IF_GRM,
    THEN_GRM,
    ELIF_GRM,
    ELSE_GRM,
    FI_GRM,
    WHILE_GRM,
    UNTIL_GRM,
    DO_GRM,
    DONE_GRM,
    SEMICOLON,
    PIPELINES,
    END
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
    //CMD_LIST,
    PIPELINE_CMD,
    WHILE_CMD,
    UNTIL_CMD,
    IF_CMD
};

struct Ast_if
{
    struct Ast *cdt;
    struct Ast *cmd1;
    struct Ast *cmd2;
};

struct Ast_simple_cmd
{
    struct Command *cmd;
};

struct Ast_pipelines
{
    struct Ast *left;
    struct Ast *right;
};

struct Ast_while
{
    struct Ast *cdt;
    struct Ast *do_cdt;
};


struct Ast_until
{
    struct Ast *cdt;
    struct Ast *do_cdt;
};

/*struct Ast_cmd_list
{
    struct Ast **list;
    size_t nbnodes;
};*/

union Ast_union
{
    struct Ast_simple_cmd sc;
    //struct Ast_cmd_list cl;
    struct Ast_pipelines pl;
    struct Ast_while wh;
    struct Ast_until ut;
    struct Ast_if aif;
};

struct Ast
{
    enum Ast_type type;
    union Ast_union data;
    struct Ast *next;
};

/*
** Fonctions exportees
*/
struct Ast *parse_command(struct Lexer *lexer);
void print_ast(struct Ast *ast, size_t depth);
struct Ast *parse_start(struct Lexer *lex);
void free_ast(struct Ast *ast);

#endif /* ! PARSING_H */
