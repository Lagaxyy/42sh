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

enum Quote
{
    SIMPLE_QUOTE,
    DOUBLE_QUOTE,
    NOT_QUOTE
};

struct Token
{
    char *str;
    size_t len;
    enum Quote quote;
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
    struct Ast_or or ;
};

struct Ast
{
    enum Ast_type type;
    union Ast_union data;
    struct Ast *next;
};

/*
** Fonctions exportées
*/
struct Ast *parse_command(struct Lexer *lexer);
struct Ast *parse_start(struct Lexer *lex);

struct Ast *simple_command(struct Lexer *lex);
struct Ast *if_command(struct Lexer *lex);
struct Ast *elif_command(struct Lexer *lex);
struct Ast *until_cmd(struct Lexer *lex);
struct Ast *while_cmd(struct Lexer *lex);
struct Ast *pipelines_command(struct Lexer *lex);
struct Ast *neg_command(struct Lexer *lex);
struct Ast *or_command(struct Lexer *lex);
struct Ast *and_command(struct Lexer *lex);

void print_ast(struct Ast *ast, size_t depth);
void free_ast(struct Ast *ast);

#endif /* ! PARSING_H */
