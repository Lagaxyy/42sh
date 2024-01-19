#ifndef _POSIX_C_SOURCE
#    define _POSIX_C_SOURCE 200809L
#endif /* ! _POSIX_C_SOURCE */

#include "lexer.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Lexer *init_lexer(char *str)
{
    struct Lexer *lexer = malloc(sizeof(struct Lexer));
    if (lexer == NULL)
        return NULL;
    
    lexer->input = str;
    lexer->pos = 0;
    return lexer;
}

void free_lexer(struct Lexer *lexer)
{
    if (lexer == NULL)
        return;
    free(lexer->input);
    free(lexer);
}

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
    if (*tok == ';')
        return false;
    // Autres conditions ??
    return true;
}

size_t strlenSep(char *str)
{
    size_t i = 0;
    while (str[i] != '\0' && str[i] != '\t' && str[i] != '\r' &&
            str[i] != '\n' && str[i] != ' ' && str[i] != ';')
    {
        i++;
    }
    return i;
}

int lexerVerif(struct Lexer *lex, enum Grammaire grm)
{
    size_t len_blank = strspn(lex->input + lex->pos, " \t\r\n");
    lex->pos += len_blank;
    // return NULL si fin de la string. ajout car sinon sa cracher
    if (lex->input[lex->pos] == 0)
        return 0;

    // Extrait le premier token
    char *token = malloc(1024);
    size_t l = 1;
    if (lex->input[lex->pos] == ';')
        // Cas special: si le premier token est ';'
        token = ";";
    else
    {
        l = strlenSep(lex->input + lex->pos);
        token = strncpy(token, lex->input + lex->pos, l);
    }

    // (TODO) Verifie la grammaire donnee (a completer au fur et a mesure)
    int check_grm = 0;
    if (grm == IF_GRM)
        check_grm = strncmp(token, "if", 2) == 0;
    else if (grm == THEN_GRM)
        check_grm = strncmp(token, "then", 4) == 0;
    else if (grm == ELSE_GRM)
        check_grm = strncmp(token, "else", 4) == 0;
    else if (grm == ELIF_GRM)
        check_grm = strncmp(token, "elif", 4) == 0;
    else if (grm == FI_GRM)
        check_grm = strncmp(token, "fi", 2) == 0;
    else if (grm == SEMICOLON)
        check_grm = strncmp(token, ";", 1) == 0;
    else
        check_grm = false;
    // Libere la memoire du token
    //free(token);
    if (check_grm == false)
    {
        return check_grm;
    }

    //augmente la position du lexer de l (le mot)
    lex->pos += l;

    return check_grm;
}

struct Token *lexer(struct Lexer *lex, enum Grammaire grm)
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
    // Skip les premiers caracteres BLANK si besoin
    size_t len_blank = strspn(lex->input + lex->pos, " \t\r\n");
    lex->pos += len_blank;

    // return NULL si fin de la string. ajout car sinon sa cracher
    if (lex->input[lex->pos] == 0)
    {
        return NULL;
    }

    // Extrait le premier token
    char *token = malloc(1024);
    size_t l = 1;
    if (lex->input[lex->pos] == ';')
        // Cas special: si le premier token est ';'
        token = ";";
    else
    {
        l = strlenSep(lex->input + lex->pos);
        token = strncpy(token, lex->input + lex->pos, l);
    }

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
    new->str = lex->input + lex->pos;
    new->len = l;

    //augmente la position du lexer de l (le mot)
    lex->pos += l;

    // Libere la memoire du token
    free(token);

    return new;
}
