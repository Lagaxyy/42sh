#ifndef PARSING_H
#define PARSING_H

#include <stddef.h>

struct Ast;
struct Lexer;

/*
** Struct et enum concernant les tokens
*/
enum Grammaire
{
    IF_GRM,
    THEN_GRM,
    ELIF_GRM,
    ELSE_GRM,
    FI_GRM,
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
    struct Ast *next;
};

/*
** Fonctions exportées
*/

/*
** Lance le parsing de l'input
*/
struct Ast *parse_command(struct Lexer *lexer);

/*
** Exécute le parsing en liant les
** fonctions propres aux features
*/
struct Ast *parse_start(struct Lexer *lex);
void print_ast(struct Ast *ast, size_t depth);
void free_ast(struct Ast *ast);

#endif /* ! PARSING_H */
