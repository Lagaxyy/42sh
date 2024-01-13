#ifndef _POSIX_C_SOURCE
#    define _POSIX_C_SOURCE 200809L
#endif /* ! _POSIX_C_SOURCE */

#include "lexer.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool check_word(char *tok)
/*
** Renvoie true si le token est un WORD
** sinon renvoie false
*/
{
    if (!tok)
        return false;
    if (*tok == 0)
        return false;
    // Autres conditions ??
    return true;
}

struct Token *lexer(char *str, enum Grammaire grm)
/*
** Considerant l'input (ici c'est str), renvoie
** un struct Token contenant un pointeur au debut
** du token et la taille du token
*/
{
    /*
    ** NB: ici je me retrouve a dupliquer les chaines
    ** de caractere pour eviter de se prendre un
    ** segmentation fault au niveau de l'utilisation
    ** de str (char *) avec strtok par exemple
    */
    // Duplique str
    char *adr_strr = strdup(str);

    // Skip les premiers caracteres BLANK si besoin
    size_t len_blank = strspn(adr_strr, " \t\r\n");
    char *strr = adr_strr + len_blank;
    // return NULL si fin de la string. ajout car sinon sa cracher
    if (*strr == 0)
    {
        free(adr_strr);
        return NULL;
    }

    // Extrait le premier token
    char *token = NULL;
    if (strr[0] == ';')
        // Cas special: si le premier token est ';'
        token = strdup(";");
    else
    {
        char *tmp = strtok(strr, " \t\r\n;");
        token = strdup(tmp);
    }

    // Libere la memoire du dup de str
    free(adr_strr);

    // (TODO) Verifie la grammaire donnee (a completer au fur et a mesure)
    bool check_grm = true;
    if (grm == WORD)
        check_grm = check_word(token);
    else
        // END
        check_grm = false;
    if (check_grm == false)
        return NULL;

    // Cree la structure Token
    struct Token *new = malloc(sizeof(struct Token));
    new->str = str + len_blank;
    new->len = strlen(token);

    // Libere la memoire du token
    free(token);

    return new;
}
