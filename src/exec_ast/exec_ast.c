#ifndef _POSIX_C_SOURCE
#    define _POSIX_C_SOURCE 200809L
#endif /* ! _POSIX_C_SOURCE */

#include "exec_ast.h"

#include <assert.h>
#include <err.h>
#include <stdlib.h>
#include <string.h>

#include "../mypipe/mypipe.h"
#include "../variable/variable.h"
#include "merge_token.h"

struct Info *info = NULL;

// Pointeur de fonction pour rediriger vers les autres
// fonctions selon le type du noeud
typedef int (*node_type)(struct Ast *ast);

static int strnint(char *str, size_t len)
{
    int val = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (str[i] < '0' || str[i] > '9')
        {
            errx(2, "Erreur convertion");
        }
        val = val * 10 + (str[i] - '0');
    }
    return val;
}

static int exec_ast_sc(struct Ast *ast)
/*
** Execute le noeud simple_command
*/
{
    if (!ast)
        return 1;
    assert(ast->type == SIMPLE_CMD);

    int rep = 1;

    // Match la commande avec sa fonction correspondante
    struct Ast_simple_cmd *sc = &ast->data.sc; // Alias pratique
    // si s'est une initialisation de variable
    struct Variable *var = varialble_parsing(sc->cmd);
    if (var != NULL)
    {
        // l'ajouter ou modifier dans info->listVar
        int err = present_or_add(&info->listVar, var);
        if (err == 0)
            return 1;
        return 0;
    }
    // merge_token(sc->cmd, info->listVar);
    struct Ast *ast_var = cpy_cmd_with_var(sc->cmd, info->listVar);
    struct Ast_simple_cmd *scvar = &ast_var->data.sc; // Alias pratique

    if (strncmp(scvar->cmd->tokenlist[0]->str, "echo", 4) == 0)
        // ECHO
        rep = my_echo(ast_var);
    else if (strncmp(scvar->cmd->tokenlist[0]->str, "true", 4) == 0)
        rep = 0;
    else if (strncmp(scvar->cmd->tokenlist[0]->str, "false", 5) == 0)
        rep = 1;
    else if (scvar->cmd->nbtoken == 2 && strncmp(scvar->cmd->tokenlist[0]->str, "exit", 4) == 0)
    {
        int cvt = strnint(scvar->cmd->tokenlist[1]->str, scvar->cmd->tokenlist[1]->len);
        exit(cvt);
    }
    else if (strncmp(scvar->cmd->tokenlist[0]->str, "continue", 8) == 0)
    {
        rep = 0;
        info->isContinue = 1;
    }
    else if (strncmp(scvar->cmd->tokenlist[0]->str, "break", 5) == 0)
    {
        rep = 0;
        info->isBreak = 1;
    }
    else
        rep = run_cmds(ast_var);

    free_cmd_var(ast_var);

    return rep;
}

static int exec_ast_if(struct Ast *ast)
/*
** Execute le noeud if_command
*/
{
    if (!ast)
        return 1;
    assert(ast->type == IF_CMD);

    struct Ast_if *aif = &ast->data.aif;

    if (!exec_ast(aif->cdt))
    {
        return exec_ast(aif->cmd1);
    }
    else
    {
        if (aif->cmd2 == NULL)
            return 0;
        return exec_ast(aif->cmd2);
    }

    /*// Alloue la memoire pour la struct Ast_simple_cmd
    struct Ast_if *aif = &ast->data.aif;*/

    return 1;
}

static int exec_ast_pipe(struct Ast *ast)
{
    if (!ast)
        return 1;
    assert(ast->type == PIPELINE_CMD);

    struct Ast_pipelines *pl = &ast->data.pl;

    return exec_pipe(pl->left, pl->right);
}

static int exec_ast_until(struct Ast *ast)
{
    if (!ast)
        return 1;
    assert(ast->type == UNTIL_CMD);

    struct Ast_until *ut = &ast->data.ut;
    int res = 0;
    // si s'est 0 continue
    while (exec_ast(ut->cdt) != 0)
    {
        res = exec_ast(ut->do_cdt);
        if(info->isContinue == 1)
        {
            info->isContinue = 0;
            continue;
        }
        if (info->isBreak == 1)
        {
            info->isBreak = 0;
            break;
        }
    }
    return res;
}

static int exec_ast_while(struct Ast *ast)
{
    if (!ast)
        return 1;
    assert(ast->type == WHILE_CMD);

    struct Ast_while *wh = &ast->data.wh;
    int res = 0;
    // si s'est 0 continue
    while (exec_ast(wh->cdt) == 0)
    {
        res = exec_ast(wh->do_cdt);
        if(info->isContinue == 1)
        {
            info->isContinue = 0;
            continue;
        }
        if (info->isBreak == 1)
        {
            info->isBreak = 0;
            break;
        }
    }
    return res;
}

static int exec_ast_and(struct Ast *ast)
{
    if (!ast)
        return 1;
    assert(ast->type == AND_CMD);

    struct Ast_and *and = &ast->data.and;

    int res = exec_ast(and->left);
    if (res != 0)
        return res;
    return exec_ast(and->right);
}

static int exec_ast_or(struct Ast *ast)
{
    if (!ast)
        return 1;
    assert(ast->type == OR_CMD);

    struct Ast_or * or = &ast->data.or ;

    int res = exec_ast(or->left);
    if (res != 0)
        return exec_ast(or->right);
    return res;
}

static int exec_ast_neg(struct Ast *ast)
{
    if (!ast)
        return 1;
    assert(ast->type == NEG_CMD);

    struct Ast_neg *neg = &ast->data.neg;

    return !exec_ast(neg->prg);
}

static int exec_ast_for(struct Ast *ast)
{
    if (!ast)
        return 1;
    assert(ast->type == FOR_CMD);

    struct Ast_for *fr = &ast->data.fr;
    struct Ast_simple_cmd *cd = &fr->valeurs->data.sc;

    if (fr->valeurs == NULL)
    {
        return 0;
    }

    struct Command *cmd = cd->cmd;

    struct Variable *var = NULL;
    int res = 0;
    char *c = NULL;
    for (size_t i = 0; i < cmd->nbtoken; i++)
    {
        // créer la variable
        var = malloc(sizeof(struct Variable));
        var->name = malloc(sizeof(struct Token));
        var->name->str = fr->var->str;
        var->name->len = fr->var->len;
        var->name->quote = fr->var->quote;

        c = malloc((cmd->tokenlist[i]->len + 1) * sizeof(char));
        c = strncpy(c, cmd->tokenlist[i]->str, cmd->tokenlist[i]->len);
        c[cmd->tokenlist[i]->len] = 0;
        var->valeur = c;

        // appeler present_or_add
        present_or_add(&info->listVar, var);
        res = exec_ast(fr->do_cdt);

        if(info->isContinue == 1)
        {
            info->isContinue = 0;
            continue;
        }
        if (info->isBreak == 1)
        {
            info->isBreak = 0;
            break;
        }
    }
    return res;
}

int exec_ast(struct Ast *ast)
/*
** Execute l'AST
** Etape: simple_command
*/
{
    if (!ast)
        return 1;

    // Redirige vers les autres fonctions selon le type du noeud
    static const node_type functions[] = {
        [SIMPLE_CMD] = &exec_ast_sc,     [IF_CMD] = &exec_ast_if,
        [PIPELINE_CMD] = &exec_ast_pipe, [WHILE_CMD] = &exec_ast_while,
        [UNTIL_CMD] = &exec_ast_until,   [AND_CMD] = &exec_ast_and,
        [OR_CMD] = &exec_ast_or,         [NEG_CMD] = &exec_ast_neg,
        [FOR_CMD] = &exec_ast_for
    };

    int i = (*functions[ast->type])(ast);
    if (info->isBreak == 1 || info->isContinue == 1 || ast->next == NULL)
    {
        return i;
    }
    else
    {
        return exec_ast(ast->next);
    }
}

int exec_ast_init(struct Ast *ast)
{
    info = malloc(sizeof(struct Info));
    info->listVar = NULL;
    info->isBreak = 0;
    info->isContinue = 0;
    int ret = exec_ast(ast);
    free_list(info->listVar);
    free(info);
    return ret;
}
