#ifndef EXEC_AST_H
#define EXEC_AST_H

#include "../parser/parsing.h"

struct ListVariable
{
    struct Variable *var;
    struct ListVariable *next;
};

/*
** Exécute l'AST
*/
int exec_ast(struct Ast *ast);

/*
** initialise la list de variable et execute l'arbre
*/
int exec_ast_init(struct Ast *ast);

/*
** Implémentation du
** builtin echo
*/
int my_echo(struct Ast *ast);

/*
** Exécute les commandes non builtin
*/
int run_cmds(struct Ast *ast);

/*
** ajoute a ListVariable la nouvelle variable
** la modifie si elle existe déjà
*/
int present_or_add(struct ListVariable **list, struct Variable *new_var);

/*
** trouve la variable dans la listVariable
*/
struct Variable *find_variable(struct ListVariable *list, struct Token *name);

/*
** free la listVariable
*/
void free_list(struct ListVariable *list);

#endif /* ! EXEC_AST_H */
