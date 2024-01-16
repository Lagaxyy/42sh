#ifndef _POSIX_C_SOURCE
#    define _POSIX_C_SOURCE 200809L
#endif /* ! _POSIX_C_SOURCE */

#include "exec_ast.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int aux_get_options(char *str, int *pN, int *iE)
/*
** Check la validite des options et met a jour
** les valeurs des pointeurs dans la cas valide
** Renvoie 0 si c'est valide, 1 sinon
*/
{
    int pN_save = *pN;
    int iE_save = *iE;

    for (size_t i = 1; i < strlen(str); i++)
    {
        if (str[i] == 'n')
            *pN = 0;
        else if (str[i] == 'E')
            *iE = 0;
        else if (str[i] == 'e')
            *iE = 1;
        else
        {
            // Restaure les anciennes valeurs
            *pN = pN_save;
            *iE = iE_save;

            return 1;
        }
    }

    return 0;
}

size_t get_options(struct Ast *ast, int *pN, int *iE)
/*
** Recupere les options de la commande
** Renvoie l'indice du premier token qui
** n'est pas une option
*/
{
    char *str = NULL;
    size_t ind = 1;
    struct Ast_simple_cmd *sc = &ast->data.sc; // Alias pratique

    // Check tous les tokens jusqu'a trouver le premier
    // token qui n'est pas une option
    for (size_t i = 1; i < sc->cmd->nbtoken; i++)
    {
        if (sc->cmd->tokenlist[i]->str[0] != '-')
            break;
        str = strdup(sc->cmd->tokenlist[i]->str);
        str[sc->cmd->tokenlist[i]->len] = '\0';
        int check_options = aux_get_options(str, pN, iE);
        if (check_options)
        {
            // Option(s) invalide(s), on print le reste
            free(str);
            break;
        }
        ind++;
        free(str);
    }

    return ind;
}

void echo_print(char *src, int iE)
/*
** Print les arguments de echo
*/
{
    for (int i = 0; src[i] != '\0'; i++)
    {
        if (iE && src[i] == '\\')
        {
            if (src[i + 1] == 'n')
            {
                putchar('\n');
                i++;
            }
            else if (src[i + 1] == 't')
            {
                putchar('\t');
                i++;
            }
            else if (src[i + 1] == '\\')
            {
                putchar('\\');
                i++;
            }
        }
        else
            putchar(src[i]);
    }
}

int my_echo(struct Ast *ast)
{
    if (!ast)
        return 1;
    assert(ast->type == SIMPLE_CMD);

    char *src = NULL;
    size_t i = 0;
    int printNewline = 1;
    int interpretEscapes = 0;
    struct Ast_simple_cmd *sc = &ast->data.sc; // Alias pratique

    size_t check_options = get_options(ast, &printNewline, &interpretEscapes);

    // Print les arguments de echo suivi d'un espace
    for (i = check_options; i < sc->cmd->nbtoken - 1; i++)
    {
        src = strdup(sc->cmd->tokenlist[i]->str);
        src[sc->cmd->tokenlist[i]->len] = '\0';
        echo_print(src, interpretEscapes);
        free(src);
        putchar(' ');
    }

    // Print le dernier argument de echo sans l'espace
    src = strdup(sc->cmd->tokenlist[i]->str);
    src[sc->cmd->tokenlist[i]->len] = '\0';
    echo_print(src, interpretEscapes);
    free(src);

    if (printNewline)
        putchar('\n');

    fflush(stdout);
    return 0;
}
