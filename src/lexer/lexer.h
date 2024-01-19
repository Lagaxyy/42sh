#ifndef LEXER_H
#define LEXER_H

#include "../parser/parsing.h"

/*
** Considerant l'input (ici c'est str), renvoie
** un struct Token contenant un pointeur au debut
** du premier token et la taille du token
*/
struct Token *lexer(char *str, int *end, enum Grammaire grm);

#endif /* ! LEXER_H */
