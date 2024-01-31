#ifndef _POSIX_C_SOURCE
#    define _POSIX_C_SOURCE 200809L
#endif /* ! _POSIX_C_SOURCE */

#include <ctype.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "exec_ast/exec_ast.h"
#include "lexer/lexer.h"
#include "parser/parsing.h"

void aux_error(void)
/*
** Fonction auxiliaire qui gère les erreurs de getopt
*/
{
    if (optopt == 'c')
        errx(2, "Argument manquant pour l'option -c");
    else if (isprint(optopt))
        errx(2, "Option inconnue -%c", optopt);
    else
        errx(2, "Caractere inconnu '\\x%x'", optopt);
}

int aux_pretty_print(char *input)
/*
** Fonction auxiliaire affichant l'AST
*/
{
    struct Lexer *lexer = init_lexer(input);
    struct Ast *ast = parse_command(lexer);
    print_ast(ast, 0);
    free_lexer(lexer);
    free_ast(ast);

    return 0;
}

int aux_verbose(void)
/*
** Fonction auxiliaire permettant l'affichage de logs
*/
{
    return 0;
}

int aux_run(char *input)
/*
** Construit l'AST de l'input et
** l'exécute
*/
{
    // Input vide
    if (!input || strlen(input) == 0)
    {
        free(input);
        return 0;
    }

    struct Lexer *lexer = init_lexer(input);
    struct Ast *ast = parse_command(lexer);
    int ret = exec_ast(ast);
    free_lexer(lexer);
    free_ast(ast);

    return ret;
}

bool present_semicolon(char *str, size_t l)
{
    int i = l - 1;
    while ( i >= 0 && (str[i] == ' ' || str[i] == '\t' || str[i] == '\r' || str[i] == '\n'))
    {
        if(str[i] != ';')
        {
            return true;
        }
        i--;
    }
    return false;
}

char *InputStandardInStr(void)
/*
** Récupère l'input depuis
** stdin
*/
{
    // Allocation dynamique pour stocker le contenu du fichier
    char *content = NULL;
    char *res = malloc(1);
    res[0] = 0;
    size_t contentSize = 0;
    size_t len = 0;
    size_t l = 0;

    // Lecture du fichier ligne par ligne
    while (getline(&content, &contentSize, stdin) != -1)
    {
        // Vérification de ligne vide
        l = strlen(content);
        if (content[l - 1] != '\n')
        {
            len += l;
            res = realloc(res, len + 1);
            strcat(res, content);
            break;
        }
        if (l > 2 && present_semicolon(content, l))
        {
            len += l;
            res = realloc(res, len + 1);
            strcat(res, content);
            res[len - 1] = '\n';
            res[len] = 0;
            continue;
        }
        if (l > 2)
        {
            // Ajout d'un point-virgule a la fin de la ligne
            len += l + 1;
            res = realloc(res, len + 1);
            strcat(res, content);
            res[len - 2] = ';';
            res[len - 1] = '\n';
            res[len] = 0;
        }
    }
    free(content);
    return res;
}

char *fileInStr(char *path)
/*
** Récupère l'input depuis
** un fichier
*/
{
    // Ouverture du fichier en mode lecture
    FILE *file = fopen(path, "r");

    // Vérification si le fichier a été ouvert avec succès
    if (file == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    // Allocation dynamique pour stocker le contenu du fichier
    char *content = NULL;
    char *res = malloc(1);
    res[0] = 0;
    size_t contentSize = 0;
    size_t len = 0;
    size_t l = 0;

    // Lecture du fichier ligne par ligne
    while (getline(&content, &contentSize, file) != -1)
    {
        // Vérification de ligne vide
        l = strlen(content);
        if (content[l - 1] != '\n')
        {
            len += l;
            res = realloc(res, len + 1);
            strcat(res, content);
            break;
        }
        if (l > 2 && present_semicolon(content, l))
        {
            len += l;
            res = realloc(res, len + 1);
            strcat(res, content);
            res[len - 1] = '\n';
            res[len] = 0;
            continue;
        }
        if (l > 2)
        {
            // Ajout d'un point-virgule a la fin de la ligne
            len += l + 1;
            res = realloc(res, len + 1);
            strcat(res, content);
            res[len - 2] = ';';
            res[len - 1] = '\n';
            res[len] = 0;
        }
    }
    free(content);

    // Fermeture du fichier
    fclose(file);

    return res;
}

char *get_input(int argc, char *argv[], char **double_dash)
/*
** Renvoie un pointeur vers l'input
** (mémoire allouée à free)
*/
{
    // Variables
    int c;
    char *filename = NULL;
    char *script = NULL;
    char *input = NULL;

    // Désactive l'affichage du message d'erreur par getopt
    opterr = 0;

    // Parse les options
    while ((c = getopt(argc, argv, "c:-:")) != -1)
    {
        switch (c)
        {
        case 'c':
            script = optarg;
            break;
        case '-':
            *double_dash = optarg;
            break;
        case '?':
            aux_error();
            break;
        default:
            errx(2, "Erreur inconnue");
        }
    }

    // Parse les arguments en plus
    for (int i = optind; i < argc; i++)
    {
        if (filename != NULL)
            errx(2, "Trop d'arguments ou arguments mal places");
        filename = argv[i];
    }

    if (script != NULL)
        // Input depuis une chaine de caractère
        input = strdup(script);
    else if (filename != NULL)
        // Input depuis un fichier
        input = fileInStr(filename);
    else
        // Input depuis stdin
        input = InputStandardInStr();

    return input;
}

int main(int argc, char **argv)
{
    // Recupère l'input
    int ret = 0;
    char *double_dash = NULL;
    char *input = get_input(argc, argv, &double_dash);

    // Redirection
    if (double_dash != NULL)
    {
        if (strcmp(double_dash, "pretty-print") == 0)
            ret = aux_pretty_print(input);
        else if (strcmp(double_dash, "verbose") == 0)
            ret = aux_verbose(); // A implémenter ?
        else
            errx(2, "Option inconnue --%s", double_dash);
    }
    else
        ret = aux_run(input);

    return ret;
}
