#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    char *input = NULL;
    if (argc < 2)
    {
        input = InputStandardInStr();
        if (input == NULL)
            return 1;
    }
    else if (strcmp(argv[1], "-c") == 0)
    {
        if (argc != 3)
        {
            fprintf(stderr, "42sh: error paramettre\n");
            return 1;
        }
        input = argv[2];
    }
    else
    {
        if (argc != 2)
        {
            fprintf(stderr, "42sh: error paramettre\n");
            return 1;
        }
        input = fileInStr(argv[1]);
        if (input == NULL)
            return 1;
    }
    printf("%s\n", input);
    //execute la fonction avec input
    free(input);
    return 0;
}
