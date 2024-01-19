#include "parsing.h"

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <string.h>

#include "../lexer/lexer.h"

void ajout_simple_cmd(struct Ast_cmd_list *cl, struct Ast *ast)
/*
** Ajoute un noeud a la command_list
*/
{
    cl->list =
        realloc(cl->list, sizeof(struct Ast *) * (cl->nbnodes + 1));
    cl->list[cl->nbnodes] = ast;
    cl->nbnodes++;
}

size_t total_len_token(struct Ast *ast)
/*
** Renvoie la somme de la longueur
** des tokens d'une simple_command
*/
{
    if (!ast)
        return 0;
    assert(ast->type == SIMPLE_CMD);

    struct Ast_simple_cmd *sc = &ast->data.sc; // Alias pratique
    size_t total = 0;

    for (size_t i = 0; i < sc->cmd->nbtoken; i++)
        total += sc->cmd->tokenlist[i]->len;

    return total;
}

size_t strlen_no_blank(const char *s)
{
    size_t count = 0;
    for (int i = 0; s[i] != '\0'; i++)
    {
        if (s[i] != ' ' && s[i] != '\t')
        count++;
    }
    return count;
}

char *super_decale_str(struct Ast *ast)
/*
** Renvoie un pointeur sur l'input
** mis a jour en fonction de tous
** les tokens du noeud
*/
{
    struct Ast_simple_cmd *sc = &ast->data.sc; // Alias pratique
    size_t nbt = sc->cmd->nbtoken;
    struct Token **tl = sc->cmd->tokenlist;
    return tl[nbt - 1]->str + tl[nbt - 1]->len;
}

struct Ast *command_list(char *str)
/*
** Renvoie un noeud de l'AST de type
** command_list
**
** Grammaire:
**
** list =
**      simple_command { ';' simple_command } [ ';' ] ;
**
** simple_command =
**      WORD { WORD } ;
*/
{
    // int end_cmd = 0;

    // Alloue la memoire pour la struct Ast et definit le type
    struct Ast *ast = malloc(sizeof(struct Ast));
    if (!ast)
        return NULL;
    ast->type = CMD_LIST;

    struct Ast_cmd_list *cl = &ast->data.cl; // Alias pratique
    cl->list = NULL;
    cl->nbnodes = 0;

    struct Ast *sub_ast = simple_command(str);
    ajout_simple_cmd(cl, sub_ast);
    size_t len_no_blank = strlen_no_blank(str);
    size_t total = total_len_token(sub_ast);

    while (total < len_no_blank)
    {
        str = super_decale_str(cl->list[cl->nbnodes - 1]);
        sub_ast = simple_command(str);
        ajout_simple_cmd(cl, sub_ast);
        total += total_len_token(sub_ast);
    }

    return ast;
}
