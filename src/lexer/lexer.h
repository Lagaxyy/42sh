#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include <stdio.h>

#include "../parser/parsing.h"

struct Lexer
{
    char *input;
    size_t pos;
};

/*
** construit le lexer a partir de l'entrée str
** inicialise la position a 0
*/
struct Lexer *init_lexer(char *str);

/*
** free le lexer
*/
void free_lexer(struct Lexer *lexer);

int lexerVerif(struct Lexer *lex, enum Grammaire grm);

bool lexer_chek(struct Lexer *lex);

/*
** Considerant l'input (ici c'est str), renvoie
** un struct Token contenant un pointeur au debut
** du premier token et la taille du token
*/
struct Token *lexer(struct Lexer *lex, enum Grammaire grm);

#endif /* ! LEXER_H */
