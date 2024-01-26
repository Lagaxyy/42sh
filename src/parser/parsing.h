#ifndef PARSING_H
#define PARSING_H

#include <stdbool.h>
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
    NEG_GRM,
    AND_GRM,
    OR_GRM,
    SEMICOLON,
    PIPELINES,
    END
};

struct Token
{
    char *str;
    size_t len;
    bool is_printVar;
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
    NEG_CMD,
    AND_CMD,
    OR_CMD,
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

struct Ast_neg
{
    struct Ast *prg;
};

struct Ast_and
{
    struct Ast *left;
    struct Ast *right;
};

struct Ast_or
{
    struct Ast *left;
    struct Ast *right;
};

union Ast_union
{
    struct Ast_simple_cmd sc;
    struct Ast_pipelines pl;
    struct Ast_while wh;
    struct Ast_until ut;
    struct Ast_if aif;
    struct Ast_neg neg;
    struct Ast_and and;
    struct Ast_or or;
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
