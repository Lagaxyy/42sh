#include "parsing.h"

#include <stdio.h>

int main(void)
{
    struct Ast *test = command_list("echo Hello ok; echo ok;");
    print_ast(test, 0);
    free_ast(test);
    return 0;
}
