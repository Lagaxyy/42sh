#ifndef EXEC_AST_H
#define EXEC_AST_H

#include "../parser/parsing.h"

/*
** Exécute l'AST
*/
int exec_ast(struct Ast *ast);

/*
** Implémentation du
** builtin echo
*/
int my_echo(struct Ast *ast);

/*
** Exécute les commandes non builtin
*/
int run_cmds(struct Ast *ast);

#endif /* ! EXEC_AST_H */
