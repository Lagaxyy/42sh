#ifndef PARSING_H
#define PARSING_H

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
    int len;
};


struct Command
{
    struct token **token;
    int nbtoken;
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

#endif /* ! PARSING_H */