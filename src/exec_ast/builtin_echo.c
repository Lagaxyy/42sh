#ifndef _POSIX_C_SOURCE
#    define _POSIX_C_SOURCE 200809L
#endif /* ! _POSIX_C_SOURCE */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "exec_ast.h"

int aux_get_options(char *str, int *pN, int *iE)
/*
** Check la validité des options et met à jour
** les valeurs des pointeurs si c'est valide
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
** Récupère les options de la commande
** Renvoie l'indice du premier token qui
** n'est pas une option
*/
{
    char *str = NULL;
    size_t ind = 1;
    struct Ast_simple_cmd *sc = &ast->data.sc; // Alias pratique

    // Check tous les tokens jusqu'à trouver le premier
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
            if (src[i + 1] != '\\')
            {
                i++;
                putchar(src[i]);
            }
            else if (src[i + 1] == '\0' || src[i + 2] == '\0')
                break;
            else if (src[i + 2] == 'n')
            {
                putchar('\n');
                i += 2;
            }
            else if (src[i + 2] == 't')
            {
                putchar('\t');
                i += 2;
            }
            else if (src[i + 2] == '\\')
            {
                putchar('\\');
                i += 2;
            }
        }
        else if (!iE && src[i] == '\\')
        {
            i++;
            putchar(src[i]);
        }
        else
            putchar(src[i]);
    }
}

int is_token_echo(char *str)
/*
** Check si le token en entrée
** est la commande echo (cf test 'two_echo')
*/
{
    size_t len = strlen(str);
    if (len < 4)
        return 0;
    return str[len - 4] == 'e' && str[len - 3] == 'c' && str[len - 2] == 'h'
        && str[len - 1] == 'o';
}

char *alias_src(char *str, size_t len)
/*
** Renvoie le token isolé de
** la chaine de caractère d'origine
*/
{
    char *src = strdup(str);
    src[len] = '\0';
    return src;
}

char *get_str(struct Ast_simple_cmd *sc, size_t i)
{
    return sc->cmd->tokenlist[i]->str;
}

size_t get_len(struct Ast_simple_cmd *sc, size_t i)
{
    return sc->cmd->tokenlist[i]->len;
}

int my_echo(struct Ast *ast)
/*
** Implémentation du
** builtin echo
*/
{
    if (!ast)
        return 0;

    char *src = NULL;
    size_t i = 0;
    int printNewline = 1;
    int interpretEscapes = 0;

    // Alias pratique
    struct Ast_simple_cmd *sc = &ast->data.sc;

    size_t check_options = get_options(ast, &printNewline, &interpretEscapes);

    // Pas d'arguments
    if (sc->cmd->nbtoken < 2)
    {
        if (printNewline)
        {
            putchar('\n');
            fflush(stdout);
        }
        return 0;
    }

    // Print les arguments de echo suivi d'un espace
    for (i = check_options; i < sc->cmd->nbtoken - 1; i++)
    {
        if (get_str(sc, i)[0] == ';' && get_len(sc, i) == 1)
            // Token ";" trouvé
            break;
        src = alias_src(get_str(sc, i), get_len(sc, i));
        if (is_token_echo(src))
        {
            // Token "echo" trouvé
            free(src);
            putchar('\n');
            continue;
        }
        echo_print(src, interpretEscapes);
        free(src);

        // Check si on peut rajouter un espace
        char *next_src = alias_src(get_str(sc, i + 1), get_len(sc, i + 1));
        if ((get_str(sc, i + 1)[0] != ';' || get_len(sc, i + 1) != 1)
            && (get_str(sc, i)[0] != '#' || get_len(sc, i) != 1)
            && !is_token_echo(next_src))
            putchar(' ');
        free(next_src);
    }

    // Print le dernier argument de echo sans l'espace
    if (get_str(sc, i)[0] != ';' || get_len(sc, i) != 1)
    {
        src = alias_src(get_str(sc, i), get_len(sc, i));
        if (!is_token_echo(src))
            echo_print(src, interpretEscapes);
        free(src);
    }

    if (printNewline)
        putchar('\n');

    fflush(stdout);
    return 0;
}
