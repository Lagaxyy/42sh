#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdlib.h>
#include <string.h>

#include "../../lexer/lexer.h"
#include "../parsing.h"

TestSuite(parser);

Test(parser, dummy)
{
    cr_assert(1);
}

/*
Test(parser, simple_cmd_simple)
{
    char *cmd = "echo foo";
    struct Ast *test_ast = parse_command(cmd);

    cr_redirect_stdout();
    print_ast(test_ast, 0);
    fflush(NULL);

    char *expected_output =
        "SIMPLE_CMD\n  Command:\n    Token 1: echo\n    Token 2: foo\n";

    // cr_expect_stdout_eq_str(expected_output);

    free_ast(test_ast);
    cr_assert(1);
}

Test(parser, simple_cmd_simple2)
{
    char *cmd = "echo Hello World!;";
    struct Ast *test_ast = parse_command(cmd);

    cr_redirect_stdout();
    print_ast(test_ast, 0);
    fflush(NULL);

    char *expected_output =
        "SIMPLE_CMD\n  Command:\n    Token 1: echo\n    Token 2: Hello\n    "
        "Token 3: World!\n    Token 4: ;\n";

    // cr_expect_stdout_eq_str(expected_output);

    free_ast(test_ast);
    cr_assert(1);
}

Test(parser, cmd_list)
{
    char *cmd = "echo Hello; echo World;echo!;";
    struct Ast *test_ast = parse_command(cmd);

    cr_redirect_stdout();
    print_ast(test_ast, 0);
    fflush(NULL);

    // char *expected_output = "Hello\nWorld\n!\n";

    // cr_expect_stdout_eq_str(expected_output);

    free_ast(test_ast);
    cr_assert(1);
}
*/
