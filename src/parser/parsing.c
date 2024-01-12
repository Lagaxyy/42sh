#include <stdio.h>
#include <stdlib.h>
#include "parsing.h"

struct Token *lexer(char *str, enum Grammaire grammaire)
{
    return NULL;
}

struct Ast *ast_new(enum Ast_type type)
{
    struct Ast *new = calloc(1, sizeof(struct Ast));
    if (!new)
        return NULL;
    new->type = type;
    return new;
}

struct Command *int_command()
{
    struct Command *c = malloc(sizeof(struct Command));
    c->nbtoken = 0;
    c->token = NULL;
    return c;
}
// ajoute un word a la struct command
void ajout_command(struct Command *c, struct Token *token)
{
    c->token = realloc(c->token, sizeof(struct Token*) * (c->nbtoken + 1));
    c->nbtoken++;
}


char *decale_str(char *str, struct Token *token)
{
    return token->str + token->len;
}

/*
simple_command =
        word {word}
*/
struct Ast *simple_command(char *str)
{
    // word
    struct Token *token = lexer(str, WORD); // return NULL si incorect
    if (token == NULL)
    {
        return NULL;
    }
    struct Ast *ast = malloc(sizeof(struct Ast));
    ast->command = int_command();
    ajout_command(ast->command, token);
    str = decale_str(str, token);

    // {word}
    token = lexer(str, WORD);
    while (token != NULL)
    {
        ajout_command(ast->command, token);
        str = decale_str(str, token);
        token = lexer(str, WORD);
    }
    return ast;
}

/*
input = 
        simple_command EOF
*/
// return l'ast si true, NULL si false
struct Ast *input(char *str)
{
    // simple_command EOF
    struct Ast *ast = simple_command(str);
    if (ast != NULL)
    {
        return ast;
    }
    return NULL;
}

void parse_command(char *str)
{
    struct Ast *ast = input(str);
    if (ast == NULL)
    {
        printf("erreur\n");
        return;
    }
}

int main(void)
{
    parse_command("echo 1");
    return 1;
}