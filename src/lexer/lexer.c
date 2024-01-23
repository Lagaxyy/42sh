#ifndef _POSIX_C_SOURCE
#    define _POSIX_C_SOURCE 200809L
#endif /* ! _POSIX_C_SOURCE */

#include "lexer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Lexer *init_lexer(char *str)
/*
** Construit le lexer à partir de l'entrée str
** et initialise la position à 0
*/
{
    struct Lexer *lexer = malloc(sizeof(struct Lexer));
    if (lexer == NULL)
        return NULL;

    lexer->input = str;
    lexer->pos = 0;
    return lexer;
}

void free_lexer(struct Lexer *lexer)
/*
** Libère la mémoire allouée par
** le lexer
*/
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

int lexerVerif(struct Lexer *lex, enum Grammaire grm)
/*
** Récupère le premier token et
** vérifie si sa grammaire est celle demandée
*/
{
    // Skip les premiers caractères BLANK si besoin
    size_t len_blank = strspn(lex->input + lex->pos, " \t\r\n");
    lex->pos += len_blank;

    // Fin de la string
    if (lex->input[lex->pos] == 0)
    {
        if (grm == END)
            // Fin attendue
            return 1;
        return 0;
    }

    // Extrait le premier token
    //   Calcule la taille du token
    size_t dynamic_size = strcspn(lex->input + lex->pos, " \t\r\n");
    char *token = malloc(dynamic_size + 1);
    size_t l = 1;
    if (lex->input[lex->pos] == ';')
    {
        // Cas spécial: si le premier token est ';'
        token[0] = ';';
        token[1] = 0;
    }
    else
    {
        l = strcspn(lex->input + lex->pos, " \t\r\n;");
        token = strncpy(token, lex->input + lex->pos, l);
    }

    // (TODO) Vérifie la grammaire donnée (à compléter au fur et à mesure)
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

    // Libère la mémoire du token
    free(token);

    // Renvoie false et ne bouge pas la position
    if (check_grm == false)
    {
        return check_grm;
    }

    // Augmente la position du lexer de l (le mot)
    lex->pos += l;

    return check_grm;
}

bool lexer_check(struct Lexer *lex)
/*
** Renvoie true si c'est un token
** spécial, sinon renvoie false
*/
{
    // Skip les premiers caractères BLANK si besoin
    size_t len_blank = strspn(lex->input + lex->pos, " \t\r\n");
    lex->pos += len_blank;

    if (strncmp(lex->input + lex->pos, "then", 4) == 0)
    {
        return true;
    }
    if (strncmp(lex->input + lex->pos, "elif", 4) == 0)
    {
        return true;
    }
    if (strncmp(lex->input + lex->pos, "else", 4) == 0)
    {
        return true;
    }
    if (strncmp(lex->input + lex->pos, "fi", 2) == 0)
    {
        return true;
    }
    return false;
}

struct Token *lexer(struct Lexer *lex, enum Grammaire grm)
/*
** Considerant l'input (ici c'est str), renvoie
** un struct Token contenant un pointeur au début
** du token et la taille du token
*/
{
    // Skip les premiers caractères BLANK si besoin
    size_t len_blank = strspn(lex->input + lex->pos, " \t\r\n");
    lex->pos += len_blank;

    // Renvoie NULL si fin de la string
    if (lex->input[lex->pos] == 0)
    {
        return NULL;
    }

    // Extrait le premier token
    //   Calcule la taille du token
    size_t dynamic_size = strcspn(lex->input + lex->pos, " \t\r\n");
    char *token = malloc(dynamic_size + 1);
    size_t l = 1;
    if (lex->input[lex->pos] == ';')
    {
        // Cas spécial: si le premier token est ';'
        token[0] = ';';
        token[1] = 0;
    }
    else
    {
        l = strcspn(lex->input + lex->pos, " \t\r\n;");
        token = strncpy(token, lex->input + lex->pos, l);
    }

    // (TODO) Vérifie la grammaire donnée (à compléter au fur et à mesure)
    bool check_grm = true;
    if (grm == WORD)
        check_grm = check_word(token);
    else
        // END
        check_grm = false;

    // Libère la mémoire du token
    free(token);

    if (check_grm == false)
        return NULL;

    // Crée la structure Token
    struct Token *new = malloc(sizeof(struct Token));
    new->str = lex->input + lex->pos;
    new->len = l;

    // Augmente la position du lexer de l (le mot)
    lex->pos += l;

    return new;
}
