#include "exec_ast.h"
#include "../parser/parsing.h"

#include <stdio.h>

int main(void)
{
    struct Ast *test = parse_command("echo -n -e Hello World! \\n");
    print_ast(test, 0);
    exec_ast(test);
    free_ast(test);
    return 0;
}
