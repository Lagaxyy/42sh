#include "merge_token.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../parser/parsing.h"
#include "../lexer/lexer.h"
#include "exec_ast.h"
#include "../variable/variable.h"

bool is_variable(struct Token *token)
{
    if (token->len >= 2 && token->str[0] == '$')
    {
        return true;
    }
    return false;
    
}

void delet_token_command(struct Command *command, size_t i)
{
    // supime le token a la possition i
    size_t ind = i;
    free(command->tokenlist[i]);
    while (ind + 1 < command->nbtoken)
    {
        command->tokenlist[ind] = command->tokenlist[ind + 1];
        ind++;
    }
    command->nbtoken--;
    command->tokenlist = realloc(command->tokenlist,command->nbtoken);
}

void insert_token_command(struct Command *command, size_t i, struct Token *token)
{
    size_t ind = command->nbtoken;
    command->tokenlist = realloc(command->tokenlist,command->nbtoken + 1);
    while (ind > i)
    {
        command->tokenlist[ind] = command->tokenlist[ind - 1];
        ind--;
    }
    command->tokenlist[i] = token;
    command->nbtoken++;
}

void merge_token(struct Command *command, struct ListVariable *ListVariable)
{
    size_t i = 0;
    struct Token token;
    struct Token *p;
    struct Lexer *lex;
    struct Variable *variable;
    while (i < command->nbtoken)
    {
        if (is_variable(command->tokenlist[i]) && command->tokenlist[i]->is_printVar)
        {
            token.str = command->tokenlist[i]->str + 1;
            token.len = command->tokenlist[i]->len - 1;
            variable = find_variable(ListVariable, &token);
            if (variable == NULL || variable->valeur[0] == 0)
            {
                // il faut suprimer le token
                delet_token_command(command, i);
                break;
            }
            lex = init_lexer(variable->valeur);
            p = lexer(lex,WORD);  // a gere si c'est null
            if (p == NULL)
            {
                delet_token_command(command, i);
                break;
            }
            free(command->tokenlist[i]);
            command->tokenlist[i] = p;
            p = lexer(lex,WORD);
            while (p != NULL)
            {
                i++;
                insert_token_command(command,i,p);
                p = lexer(lex,WORD);
            }
        }
        i++;
    }
}
