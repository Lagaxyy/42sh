#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>

#include "../parser/parsing.h"

struct Lexer
{
    char *input;
    size_t pos;
};

/*
** Construit le lexer à partir de l'entrée str
** et initialise la position à 0
*/
struct Lexer *init_lexer(char *str);

/*
** Libère la mémoire allouée par
** le lexer
*/
void free_lexer(struct Lexer *lexer);

/*
** Récupère le premier token et
** vérifie si sa grammaire est celle demandée
*/
int lexerVerif(struct Lexer *lex, enum Grammaire grm);

/*
** Renvoie true si c'est un token
** spécial, sinon renvoie false
*/
bool lexer_check(struct Lexer *lex);

/*
** Considerant l'input (ici c'est str), renvoie
** un struct Token contenant un pointeur au debut
** du token et la taille du token
*/
struct Token *lexer(struct Lexer *lex, enum Grammaire grm);

#endif /* ! LEXER_H */
