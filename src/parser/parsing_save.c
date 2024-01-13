#include <stdio.h>
#include <stdlib.h>
#include "../lexer/lexer.h"
#include "parsing.h"

/*
struct Token *lexer(char *str, enum Grammaire grammaire)
{
    return NULL;
}
*/
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
    c->token[c->nbtoken] = token;
    c->nbtoken++;
}


char *decale_str(struct Token *token)
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
    struct Ast *ast = ast_new(COMAND);
    ast->command = int_command();
    ajout_command(ast->command, token);
    str = decale_str(token);

    // {word}
    token = lexer(str, WORD);
    while (token != NULL)
    {
        ajout_command(ast->command, token);
        str = decale_str(token);
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

void printAst(struct Ast *ast, size_t depth)
{
    // Print the type of the current node
    for (size_t i = 0; i < depth; i++)
        printf("  "); // Add indentation based on depth

    switch (ast->type)
    {
    case IF:
        printf("IF\n");
        break;
    case THEN:
        printf("THEN\n");
        break;
    case ELIF:
        printf("ELIF\n");
        break;
    case ELSE:
        printf("ELSE\n");
        break;
    case FI:
        printf("FI\n");
        break;
    case COMAND:
        printf("COMMAND\n");
        break;
    default:
        printf("Unknown Type\n");
        break;
    }

    // Print the command if present
    if (ast->command != NULL)
    {
        for (size_t i = 0; i < depth + 1; i++)
            printf("  ");

        printf("Command:\n");

        for (size_t i = 0; i < ast->command->nbtoken; i++)
        {
            for (size_t j = 0; j < depth + 2; j++)
                printf("  ");
            int s = ast->command->token[i]->len;
            printf("Token %zu: %.*s\n", i + 1, s, ast->command->token[i]->str);
            //printf("Token %ld: %s\n", i + 1, ast->command->token[i]->str);
        }
    }

    // Recursively print child nodes
    for (size_t i = 0; i < ast->nbchild; i++)
    {
        printAst(ast->child[i], depth + 1);
    }

    // Print the next node in the list
    if (ast->next != NULL)
    {
        printf("\n");
        printAst(ast->next, depth);
    }
}

void parse_command(char *str)
{
    struct Ast *ast = input(str);
    if (ast == NULL)
    {
        printf("erreur\n");
        return;
    }
    printAst(ast,0);
}

int main(void)
{
    parse_command("echo 1 52");
    return 1;
}