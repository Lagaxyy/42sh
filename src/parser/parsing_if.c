#include "../lexer/lexer.h"
#include "parsing.h"

struct Ast *elif_command(struct Lexer *lex)
/*
** S'occupe de parse la commande après un 'elif'
**
** Grammaire:
**
** else_clause = ... | 'elif' compound_list 'then' compound_list [else_clause];
*/
{
    // Alloue la mémoire pour la struct Ast et définit le type
    struct Ast *ast = calloc(1, sizeof(struct Ast));
    if (!ast)
        return NULL;
    ast->type = IF_CMD;
    ast->next = NULL;

    // Alias pratique
    struct Ast_if *aif = &ast->data.aif;

    // Parse la commande
    aif->cdt = parse_start(lex);

    // Récupère la condition
    if (!aif->cdt)
    {
        free_ast(ast);
        return NULL;
    }
    // Récupère le 'then'
    if (!lexerVerif(lex, THEN_GRM))
    {
        free_ast(ast);
        return NULL;
    }
    lexerVerif(lex, SEMICOLON);
    aif->cmd1 = parse_start(lex);

    // Récupère le 'elif' ou 'else'
    if (lexerVerif(lex, ELIF_GRM))
    {
        aif->cmd2 = elif_command(lex);
    }
    else if (lexerVerif(lex, ELSE_GRM))
    {
        lexerVerif(lex, SEMICOLON);
        aif->cmd2 = parse_start(lex);
    }
    else
        aif->cmd2 = NULL;

    return ast;
}

struct Ast *if_command(struct Lexer *lex)
/*
** Renvoie un noeud de l'AST de type
** if_command
**
** Grammaire:
**
** if_command =
**      'if' compound_list 'then' compound_list [ else_clause ] 'fi' ;
*/
{
    // Si 'if' pas trouvé
    if (!lexerVerif(lex, IF_GRM))
    {
        // On continue
        return for_cmd(lex);
    }

    // Alloue la mémoire pour la struct Ast et définit le type
    struct Ast *ast = calloc(1, sizeof(struct Ast));
    if (!ast)
        return NULL;
    ast->type = IF_CMD;
    ast->next = NULL;

    // Alias pratique
    struct Ast_if *aif = &ast->data.aif;

    // Trouve la condition
    aif->cdt = parse_start(lex);
    if (!aif->cdt)
    {
        free_ast(ast);
        return NULL;
    }

    // Trouve le 'then'
    if (!lexerVerif(lex, THEN_GRM))
    {
        free_ast(ast);
        return NULL;
    }
    lexerVerif(lex, SEMICOLON);
    aif->cmd1 = parse_start(lex);

    // Trouve le 'elif' ou 'else'
    if (lexerVerif(lex, ELIF_GRM))
    {
        aif->cmd2 = elif_command(lex);
    }
    else if (lexerVerif(lex, ELSE_GRM))
    {
        lexerVerif(lex, SEMICOLON);
        aif->cmd2 = parse_start(lex);
    }
    else
        aif->cmd2 = NULL;

    // Trouve le 'fi'
    if (!lexerVerif(lex, FI_GRM))
    {
        free_ast(ast);
        return NULL;
    }

    return ast;
}
