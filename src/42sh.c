#ifndef _POSIX_C_SOURCE
#    define _POSIX_C_SOURCE 200112L
#endif /* ! _POSIX_C_SOURCE */

#include "lexer/lexer.h"
#include "parser/parsing.h"
#include "exec_ast/exec_ast.h"

#include <err.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void aux_error(void)
/*
** Fonction auxiliaire qui gere les erreurs de getopt
*/
{
    if (optopt == 'c')
        errx(2, "Argument manquant pour l'option -c");
    else if (isprint(optopt))
        errx(2, "Option inconnue -%c", optopt);
    else
        errx(2, "Caractere inconnu '\\x%x'", optopt);
}

void aux_pretty_print(char *input)
/*
** Fonction auxiliaire affichant l'AST
*/
{
    struct Ast *ast = parse_command(input);
    print_ast(ast, 0);
    free_ast(ast);

    return;
}

void aux_verbose(void)
/*
** Fonction auxiliaire permettant l'affichage de logs
*/
{
    return;
}

void aux_run(char *input)
/*
** Construit l'AST de l'input et
** l'execute
*/
{
    struct Ast *ast = parse_command(input);
    exec_ast(ast);
    free_ast(ast);

    return;
}

char *InputStandardInStr(void)
{
    char *inputStr = NULL;
    char chunk[1024];
    size_t inputSize = 0;
    while (1)
    {
        if (fgets(chunk, sizeof(chunk), stdin) == NULL)
            break;
        size_t chunkSize = strlen(chunk);
        inputStr = realloc(inputStr, inputSize + chunkSize + 1);
        if (inputStr == NULL)
        {
            fprintf(stderr, "Memory allocation failed\n");
            free(inputStr);
            return NULL;
        }
        strcpy(inputStr + inputSize, chunk);
        inputSize += chunkSize;
        if (feof(stdin))
            break;
    }
    return inputStr;
}

char *fileInStr(char *path)
{
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Cannot open file %s\n", path);
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *fileContent = malloc(fileSize + 1);
    if (fileContent == NULL)
    {
        fclose(file);
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    size_t bytesRead = fread(fileContent, 1, fileSize, file);
    fileContent[bytesRead] = '\0';
    fclose(file);
    return fileContent;
}

int main(int argc, char **argv)
{
    // Variables
    int c;
    char *double_dash = NULL;
    char *filename = NULL;
    char *input = NULL;
    char *script = NULL;

    // Desactive l'affichage du message d'erreur par getopt
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
            double_dash = optarg;
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
        // Input depuis une chaine de caractere
        input = script;
    else if (filename != NULL)
        // Input depuis un fichier
        input = fileInStr(filename);
    else
        // Input depuis stdin
        input = InputStandardInStr();

    // Redirection
    if (double_dash != NULL)
    {
        if (strcmp(double_dash, "pretty-print") == 0)
            aux_pretty_print(input);
        else if (strcmp(double_dash, "verbose") == 0)
            aux_verbose(); // A implementer ?
        else
            errx(2, "Option inconnue --%s", double_dash);
    }
    else
        aux_run(input);

    return 0;
}
