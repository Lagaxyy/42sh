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

static bool check_word(char *tok, size_t len)
/*
** Renvoie true si le token est un WORD
** sinon renvoie false
*/
{
    if (!tok || len == 0)
        return false;
    if (*tok == ';')
        return false;
    if (len == 2 && strncmp(tok, "in", 2) && strspn(tok + 2, " \n\t") > 0)
        return false;
    // si il y a une ' ouvrante mais pas fermante
    if (tok[0] == '\'' && tok[len - 1] != '\'')
        return false;
    // Autres conditions ??
    return true;
}

static size_t strlenSep(char *str)
{
    size_t i = 0;
    if (str[i] == '\'')
    {
        i++;
        while (str[i] != '\'' && str[i] != 0)
        {
            i++;
        }
        if (str[i] == '\'')
            return i + 1;
        return i;
    }
    else if (str[i] == '\"')
    {
        i++;
        while (str[i] != '\"' || str[i] == 0)
        {
            i++;
        }
        if (str[i] == '\"')
            return i + 1;
        return i;
    }
    while (str[i] != '\0' && str[i] != '\t' && str[i] != '\r' && str[i] != '\n'
           && str[i] != ' ' && str[i] != ';' && str[i] != '|'
           && strncmp(str + i, "&&", 2) != 0 && str[i] != '\''
           && str[i] != '\"')
    {
        i++;
    }
    return i;
}

static size_t skipSpace_build(char *str)
{
    size_t ind = 0;
    while (str[ind] == ' ' || str[ind] == '\t' || str[ind] == '\r'
           || str[ind] == '\n')
    {
        ind++;
    }
    return ind;
}

static size_t skipSpace(char *str)
/*
** La fonction skipSpace prend en paramètre une chaîne de caractères (str)
** La renvoie la position du prochain mot à évaluer.
** Si un caractère '#' est présent, la fonction ignore tout le contenu jusqu'à
*un '\n'.
** Si le caractère actuel fait partie des espaces " \t\r\n", la fonction passe
*au caractère suivant.
** La position renvoyée indique le début du prochain mot à évaluer dans la
*chaîne.
*/
{
    size_t rep = skipSpace_build(str);
    if (str[rep] == '#')
    {
        while (str[rep] != 0 && str[rep] != '\n')
        {
            rep++;
        }
        rep += skipSpace(str + rep);
    }
    return rep;
}

static int check_token(char *token, enum Grammaire grm)
{
    if (grm == IF_GRM)
        return strncmp(token, "if", 2) == 0 && strspn(token + 2, " \n\t") > 0;
    else if (grm == THEN_GRM)
        return strncmp(token, "then", 4) == 0 && strspn(token + 4, " \n\t") > 0;
    else if (grm == ELSE_GRM)
        return strncmp(token, "else", 4) == 0 && strspn(token + 4, " \n\t") > 0;
    else if (grm == ELIF_GRM)
        return strncmp(token, "elif", 4) == 0 && strspn(token + 4, " \n\t") > 0;
    else if (grm == FI_GRM)
        return strncmp(token, "fi", 2) == 0
            && (*(token + 2) == ';' || *(token + 2) == '\0'
                || *(token + 2) == ' ');
    else if (grm == WHILE_GRM)
        return strncmp(token, "while", 5) == 0
            && strspn(token + 5, " \n\t") > 0;
    else if (grm == UNTIL_GRM)
        return strncmp(token, "until", 5) == 0
            && strspn(token + 5, " \n\t") > 0;
    else if (grm == FOR_GRM)
        return strncmp(token, "for", 3) == 0 && strspn(token + 3, " \n\t") > 0;
    else if (grm == IN_GRM)
        return strncmp(token, "in", 2) == 0 && strspn(token + 2, " \n\t") > 0;
    else if (grm == DO_GRM)
        return strncmp(token, "do", 2) == 0 && strspn(token + 2, " \n\t") > 0;
    else if (grm == DONE_GRM)
        return strncmp(token, "done", 4) == 0
            && (*(token + 4) == ';' || *(token + 4) == '\0'
                || *(token + 2) == ' ');
    else if (grm == SEMICOLON)
        return strncmp(token, ";", 1) == 0;
    else
        return false;
}

int lexerVerif(struct Lexer *lex, enum Grammaire grm)
{
    size_t len_blank = skipSpace(lex->input + lex->pos);
    lex->pos += len_blank;
    // return NULL si fin de la string. ajout car sinon sa cracher
    if (lex->input[lex->pos] == 0)
    {
        if (grm == END)
            return 1;
        return 0;
    }

    // si s'est une pipelines
    if ((grm == PIPELINES && *(lex->input + lex->pos) == '|'
         && *(lex->input + lex->pos + 1) != '|')
        || (grm == NEG_GRM && *(lex->input + lex->pos) == '!'))
    {
        lex->pos++;
        return 1;
    }

    if ((grm == AND_GRM && strncmp(lex->input + lex->pos, "&&", 2) == 0)
        || (grm == OR_GRM && strncmp(lex->input + lex->pos, "||", 2) == 0))
    {
        lex->pos += 2;
        return 1;
    }

    // Extrait le premier token
    char *token = NULL;
    size_t l = 1;
    if (lex->input[lex->pos] == ';')
    {
        token = malloc(2 * sizeof(char));
        if (token == NULL)
            return 0;
        // Cas special: si le premier token est ';'
        token[0] = ';';
        token[1] = 0;
    }
    else
    {
        l = strlenSep(lex->input + lex->pos);
        token = malloc((l + 2) * sizeof(char));
        if (token == NULL)
            return 0;
        token = strncpy(token, lex->input + lex->pos, l + 1);
    }

    // (TODO) Verifie la grammaire donnee (a completer au fur et a mesure)
    int check_grm = check_token(token, grm);
    // Libere la memoire du token
    free(token);
    if (check_grm == false)
    {
        return check_grm;
    }

    // augmente la position du lexer de l (le mot)
    lex->pos += l;

    return check_grm;
}

bool lexer_check(struct Lexer *lex)
{
    size_t len_blank = strspn(lex->input + lex->pos, " \t\r\n");
    lex->pos += len_blank;
    if (strncmp(lex->input + lex->pos, "then", 4) == 0)
        return true;
    if (strncmp(lex->input + lex->pos, "elif", 4) == 0)
        return true;
    if (strncmp(lex->input + lex->pos, "else", 4) == 0)
        return true;
    if (strncmp(lex->input + lex->pos, "fi", 2) == 0)
        return true;
    if (strncmp(lex->input + lex->pos, "in", 2) == 0)
        return true;
    if (strncmp(lex->input + lex->pos, "do", 2) == 0)
        return true;
    if (strncmp(lex->input + lex->pos, "done", 4) == 0)
        return true;
    return false;
}

static struct Token *crate_token(struct Lexer *lex, size_t l)
{
    struct Token *new = malloc(sizeof(struct Token));
    if (*(lex->input + lex->pos) == '\'')
    {
        new->str = lex->input + lex->pos + 1;
        new->len = l - 2;
        new->quote = SIMPLE_QUOTE;
    }
    else if (*(lex->input + lex->pos) == '\"')
    {
        new->str = lex->input + lex->pos + 1;
        new->len = l - 2;
        new->quote = DOUBLE_QUOTE;
    }
    else
    {
        new->str = lex->input + lex->pos;
        new->len = l;
        new->quote = NOT_QUOTE;
    }

    // augmente la position du lexer de l (le mot)
    lex->pos += l;
    return new;
}

struct Token *lexer(struct Lexer *lex, enum Grammaire grm)
/*
** Considerant l'input (ici c'est str), renvoie
** un struct Token contenant un pointeur au debut
** du token et la taille du token
** Met a jour le pointeur end_cmd vers 1 si on
** tombe sur un ';' ou la fin de la chaine de caractere
*/
{
    /*
    ** NB: ici je me retrouve a dupliquer les chaines
    ** de caractere pour eviter de se prendre un
    ** segmentation fault au niveau de l'utilisation
    ** de str (char *) avec strtok par exemple
    */
    // Skip les premiers caracteres BLANK si besoin
    size_t len_blank = skipSpace(lex->input + lex->pos);
    lex->pos += len_blank;

    // return NULL si fin de la string. ajout car sinon sa cracher
    if (lex->input[lex->pos] == 0)
        return NULL;

    if (lex->input[lex->pos] == '|' || lex->input[lex->pos] == '!'
        || strncmp(lex->input + lex->pos, "&&", 2) == 0)
        return NULL;

    // Extrait le premier token
    char *token = NULL;
    size_t l = 1;
    if (lex->input[lex->pos] == ';')
    {
        // Cas special: si le premier token est ';'
        token = malloc(2 * sizeof(char));
        if (token == NULL)
            return NULL;
        token[0] = ';';
        token[1] = 0;
    }
    else
    {
        l = strlenSep(lex->input + lex->pos);
        token = malloc((l + 1) * sizeof(char));
        if (token == NULL)
            return NULL;
        token = strncpy(token, lex->input + lex->pos, l + 1);
    }

    // (TODO) Verifie la grammaire donnee (a completer au fur et a mesure)
    bool check_grm = true;
    if (grm == WORD)
        check_grm = check_word(token, l);
    else
        // END
        check_grm = false;
    // Libere la memoire du token
    free(token);
    if (check_grm == false)
        return NULL;

    // Cree la structure Token
    struct Token *new = crate_token(lex, l);

    return new;
}
