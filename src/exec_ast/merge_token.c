#include "merge_token.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool is_variable_tok(struct Token *token)
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
    command->tokenlist = realloc(command->tokenlist, command->nbtoken * sizeof(struct Command));
}

void insert_token_command(struct Command *command, size_t i,
                          struct Token *token)
{
    size_t ind = command->nbtoken;
    command->tokenlist = realloc(command->tokenlist, (command->nbtoken + 1) * sizeof(struct Command));
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
    size_t j = 0;
    struct Token token;
    struct Token *p;
    struct Lexer *lex;
    struct Variable *variable;
    while (i < command->nbtoken)
    {
        if (is_variable_tok(command->tokenlist[i])
            && command->tokenlist[i]->quote != SIMPLE_QUOTE)
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
            p = lexer(lex, WORD); // a gere si c'est null
            if (p == NULL)
            {
                delet_token_command(command, i);
                free(lex);
                break;
            }
            free(command->tokenlist[i]);
            command->tokenlist[i] = p;
            p = lexer(lex, WORD);
            while (p != NULL)
            {
                j++;
                insert_token_command(command, i + j, p);
                p = lexer(lex, WORD);
            }
            free(lex);
            continue;
        }
        i++;
    }
}

/*
** copy la command dans cmd
** renvoi la cmd
*/
struct Command *cpy_cmd(struct Command *cmd, struct Command *command)
{
    struct Token *token = NULL;
    struct Token *cpy = NULL;
    cmd->tokenlist = malloc((command->nbtoken + 1) * sizeof(struct Command));
    while (cmd->nbtoken < command->nbtoken)
    {
        struct Token *token = command->tokenlist[cmd->nbtoken];
        cpy = malloc(sizeof(struct Token));
        cpy->len = token->len;
        cpy->quote = token->quote;
        cpy->str = malloc((cpy->len + 1) + sizeof(char));
        cpy->str = strncpy(cpy->str, token->str, cpy->len);
        cpy->str[cpy->len] = 0;
        cmd->tokenlist[cmd->nbtoken] = cpy;
        cmd->nbtoken++;
    }
    return cmd;
}

struct Command *cpy_cmd_with_var(struct Command *command, struct ListVariable *ListVariable)
{
    size_t i = 0;
    struct Command *cmd = malloc(sizeof(struct Command));
    cmd->nbtoken = 0;
    cmd->tokenlist = NULL;
    cmd = cpy_cmd(cmd, command);

    return cmd;
}
