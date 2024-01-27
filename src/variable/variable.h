#ifndef VARIABLE_H
#define VARIABLE_H

#include "../parser/parsing.h"

struct Variable
{
    struct Token *name;
    char *valeur; // penser a free
};

struct Variable *varialble_parsing(struct Command *command);
void free_var(struct Variable *var);

#endif /* ! VARIABLE_H */
