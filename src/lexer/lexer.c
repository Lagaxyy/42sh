#ifndef _POSIX_C_SOURCE
#    define _POSIX_C_SOURCE 200809L
#endif /* ! _POSIX_C_SOURCE */

#include "lexer.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool check_word(char *tok)
{
    if (!tok)
        return false;
    return true;
}

struct Token *lexer(char *str, enum Grammaire grm)
{
    // Duplique la chaine de caractere
    char *adr_strr = strdup(str);

    // Skip les premiers ' ' si besoin
    size_t len_spc = strspn(adr_strr, " ");
    char *strr = adr_strr + len_spc;

    // Extrait le premier token
    char *token = NULL;
    if (str[0] == ';')
        token = strdup(";");
    else
    {
        char *tmp = strtok(strr, " ;");
        token = strdup(tmp);
    }

    // Libere la memoire de strdup
    free(adr_strr);

    // (TODO) Verifie la grammaire donnee
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
    new->str = str + len_spc;
    new->len = strlen(token);

    // Libere la memoire du token
    free(token);

    return new;
}
