#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Enum pour les différentes conditions
enum Condition {
    EMPTY, IF, THEN, ELIF, ELSE, FI, PVIRG, NEWLINE, QUOTE, WORD
};

// Structure pour stocker le token
struct Token {
    enum Condition condition;
    char* word;
};

// Tableau global de noms de conditions
const char *condition_names[] = {
    "EMPTY","IF", "THEN", "ELIF", "ELSE", "FI", "PVIRG", "NEWLINE", "QUOTE", "WORD", ""
};

struct Token init_token();
void free_list_tokens(struct Token* tokenList, int numTokens);
void fnmatch_for_tokens(struct Token* tokenList, int *numTokens, char *str);
struct Token* char_to_token(char* str, int* numTokens);
void free_tokens(struct Token* tokenList, int numTokens);


#endif /* ! LEXER_H */
