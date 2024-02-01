#include <stdio.h>

#include "../parser/parsing.h"

void print_ast(struct Ast *ast, size_t depth);

static void print_space(size_t depth)
{
    for (size_t i = 0; i < depth; ++i)
    {
        putchar(' '); // Indentation pour représenter la profondeur de l'arbre
    }
}

static void print_simple_cmd(struct Ast *ast, size_t depth)
{
    printf("SIMPLE COMMAND\n");
    // Alias pratique
    struct Ast_simple_cmd *sc = &ast->data.sc;

    // Print la commande si elle existe
    if (ast->type == SIMPLE_CMD && sc->cmd != NULL)
    {
        print_space(depth + 2);

        printf("Command:\n");

        for (size_t i = 0; i < sc->cmd->nbtoken; i++)
        {
            print_space(depth + 2);
            int s = sc->cmd->tokenlist[i]->len;
            if (sc->cmd->tokenlist[i]->quote == SIMPLE_QUOTE)
                printf("Token %zu: '%.*s'\n", i + 1, s,
                       sc->cmd->tokenlist[i]->str);
            else if ((sc->cmd->tokenlist[i]->quote == DOUBLE_QUOTE))
                printf("Token %zu: \"%.*s\"\n", i + 1, s,
                       sc->cmd->tokenlist[i]->str);
            else
                printf("Token %zu: %.*s\n", i + 1, s,
                       sc->cmd->tokenlist[i]->str);
        }
    }
}

static void print_if_cmd(struct Ast *ast, size_t depth)
{
    printf("IF\n");
    print_ast(ast->data.aif.cdt, depth + 2);
    print_space(depth);
    printf("THEN\n");
    print_ast(ast->data.aif.cmd1, depth + 2);
    print_space(depth);
    printf("ELSE\n");
    print_ast(ast->data.aif.cmd2, depth + 2);
}

static void print_pipeline_cmd(struct Ast *ast, size_t depth)
{
    printf("PIPELINES\n");
    print_space(depth);
    printf("left");
    print_ast(ast->data.pl.left, depth + 2);
    print_space(depth);
    printf("right");
    print_ast(ast->data.pl.right, depth + 2);
}

static void print_for_cmd(struct Ast *ast, size_t depth)
{
    printf("FOR\n");
    print_space(depth + 2);
    int s = ast->data.fr.var->len;
    printf("Token %d: %.*s\n", 1, s, ast->data.fr.var->str);
    print_space(depth);
    printf("IN\n");
    print_ast(ast->data.fr.valeurs, depth + 2);
    print_space(depth);
    printf("DO\n");
    print_ast(ast->data.fr.do_cdt, depth + 2);
}

static void print_while_cmd(struct Ast *ast, size_t depth)
{
    printf("WHILE\n");
    print_ast(ast->data.wh.cdt, depth + 2);
    print_space(depth);
    printf("DO\n");
    print_ast(ast->data.wh.do_cdt, depth + 2);
}

static void print_until_cmd(struct Ast *ast, size_t depth)
{
    printf("WHILE\n");
    print_ast(ast->data.wh.cdt, depth + 2);
    print_space(depth);
    printf("DO\n");
    print_ast(ast->data.wh.do_cdt, depth + 2);
}

static void print_and_cmd(struct Ast *ast, size_t depth)
{
    printf("AND\n");
    print_space(depth);
    printf("left");
    print_ast(ast->data.and.left, depth + 2);
    print_space(depth);
    printf("right");
    print_ast(ast->data.and.right, depth + 2);
}

static void print_or_cmd(struct Ast *ast, size_t depth)
{
    printf("OR\n");
    print_space(depth);
    printf("left");
    print_ast(ast->data.or.left, depth + 2);
    print_space(depth);
    printf("right");
    print_ast(ast->data.or.right, depth + 2);
}

static void print_neg_cmd(struct Ast *ast, size_t depth)
{
    printf("NEG\n");
    print_ast(ast->data.neg.prg, depth + 2);
}

void print_ast(struct Ast *ast, size_t depth)
{
    if (!ast)
        return;
    print_space(depth);
    switch (ast->type)
    {
    case IF_CMD:
        print_if_cmd(ast, depth);
        break;
    case PIPELINE_CMD:
        print_pipeline_cmd(ast, depth);
        break;
    case SIMPLE_CMD:
        print_simple_cmd(ast, depth);
        break;
    case FOR_CMD:
        print_for_cmd(ast, depth);
        break;
    case WHILE_CMD:
        print_while_cmd(ast, depth);
        break;
    case UNTIL_CMD:
        print_until_cmd(ast, depth);
        break;
    case AND_CMD:
        print_and_cmd(ast, depth);
        break;
    case OR_CMD:
        print_or_cmd(ast, depth);
        break;
    case NEG_CMD:
        print_neg_cmd(ast, depth);
        break;
    default:
        fprintf(stderr, "Type d'AST inconnu\n");
        break;
    }
    // Appel récursif pour imprimer les nœuds suivants dans la liste
    print_ast(ast->next, depth);
}
