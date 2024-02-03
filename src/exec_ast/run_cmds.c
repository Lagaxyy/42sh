#ifndef _POSIX_C_SOURCE
#    define _POSIX_C_SOURCE 200809L
#endif /* ! _POSIX_C_SOURCE */

#include <assert.h>
#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "exec_ast.h"

static char *write_sh(void)
{
    char *c = strdup("./src/42sh");
    return c;
}

int run_cmds(struct Ast *ast)
/*
** Exécute les commandes non builtin
*/
{
    if (!ast)
        return 0;
    assert(ast->type == SIMPLE_CMD);

    // Variables
    struct Ast_simple_cmd *sc = &ast->data.sc; // Alias pratique
    size_t nbt = sc->cmd->nbtoken;
    char *str = NULL;
    int exitcode = 0;

    // Construction d'un argv à partir de
    // la liste de tokens
    char **argv = malloc(sizeof(char *) * (nbt + 1));
    for (size_t i = 0; i < nbt; i++)
    {
        if (sc->cmd->tokenlist[i]->str[0] == '.'
            && sc->cmd->tokenlist[i]->len == 1 && i == 0)
        {
            argv[0] = write_sh();
            continue;
        }
        str = strdup(sc->cmd->tokenlist[i]->str);
        str[sc->cmd->tokenlist[i]->len] = '\0';
        argv[i] = str;
    }
    argv[nbt] = NULL; // NULL marque la fin de l'array

    // Exécute la commande
    int pid = fork();
    if (pid == 0)
    {
        // Processus enfant
        execvp(argv[0], argv);
        if (errno == EACCES)
            errx(126, "Permission refusee");
        errx(127, "Commande inconnue: \n=> %s", argv[0]);
    }
    else
        // Processus parent
        waitpid(pid, &exitcode, 0);

    // Libère la mémoire allouée pour l'argv
    for (size_t j = 0; j < nbt; j++)
    {
        if (argv[j])
            free(argv[j]);
    }
    free(argv);

    return WEXITSTATUS(exitcode);
}
