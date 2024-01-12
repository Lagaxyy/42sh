#include <stdio.h>
#include <string.h>

int my_echo(char *option, char *src)
{
    int printNewline = 1;
    if (option != NULL && strcmp(option, "-n") == 0)
        printNewline = 0;
    int interpretEscapes = 0;
    if (option != NULL && strcmp(option, "-e") == 0)
        interpretEscapes = 1;
    else if (option != NULL && strcmp(option, "-E") == 0)
        interpretEscapes = 0;
    for (int i = 0; src[i] != '\0'; i++)
    {
        if (interpretEscapes && src[i] == '\\')
        {
            if (src[i + 1] == 'n')
            {
                putchar('\n');
                i++;
            }
            else if (src[i + 1] == 't')
            {
                putchar('\t');
                i++;
            }
            else if (src[i + 1] == '\\')
            {
                putchar('\\');
                i++;
            }
        }
        else
            putchar(src[i]);
    }
    if (printNewline)
        putchar('\n');
    fflush(stdout);
    return 0;
}
