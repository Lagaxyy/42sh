#include "parsing.h"

#include <err.h>

#include "../lexer/lexer.h"

struct Ast *parse_start(struct Lexer *lex)
/*
** Exécute le parsing sur l'input
*/
{
    // Parse la commande
    struct Ast *ast = and_command(lex);

    if (ast == NULL)
        // Pas d'AST
        return NULL;
    if (!lexerVerif(lex, SEMICOLON))
        // Prochain token n'est pas un ';'
        return ast;
    if (!lexerVerif(lex, END) && lexer_check(lex) == false)
    {
        // Parse la prochaine commande
        struct Ast *next_ast = parse_start(lex);
        if (!next_ast)
        {
            // Prochaine commande vide
            free_ast(ast);
            return NULL;
        }
        ast->next = next_ast;
    }
    return ast;
}

struct Ast *parse_command(struct Lexer *lexer)
/*
** Lance l'exécution du parsing
*/
{
    struct Ast *ast = parse_start(lexer);
    if (!ast)
    {
        if (lexer->input[0] == '#')
            // Cas spécial: uniquement un commentaire
            return NULL;
        errx(2, "Erreur lors de la construction de l'AST");
    }

    return ast;
}
