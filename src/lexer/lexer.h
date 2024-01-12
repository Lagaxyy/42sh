#ifndef LEXER_H
#define LEXER_H

#include "../parser/parsing.h"

/*
** Renvoie le premier token de la chaine de caractere
** ou NULL si le token ne correspond pas a la grammaire demandee
*/
struct Token *lexer(char *str, enum Grammaire grm);

#endif /* ! LEXER_H */
