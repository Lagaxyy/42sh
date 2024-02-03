#ifndef _POSIX_C_SOURCE
#    define _POSIX_C_SOURCE 200809L
#endif /* ! _POSIX_C_SOURCE */

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdlib.h>
#include <string.h>

#include "../../lexer/lexer.h"
#include "../parsing.h"

// Coverage: 93.7% (moyenne) (02/02/2024)
TestSuite(parser);

Test(parser, dummy)
{
    cr_assert(1);
}

// === Tests parsing ==========================================================

Test(parser, comment_only)
{
    char *input = strdup("# Test");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);

    free_lexer(lexer);

    if (!test_ast)
        cr_assert(1);
    else
    {
        free_ast(test_ast);
        cr_assert(0);
    }
}

Test(parser, command_list_simple)
{
    char *input = strdup("echo Hello; echo World!;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);

    cr_redirect_stdout();
    print_ast(test_ast, 0);
    fflush(NULL);

    char *expected_output =
        "SIMPLE COMMAND\n  Command:\n  Token 1: echo\n  Token 2: Hello\nSIMPLE COMMAND\n  Command:\n  Token 1: echo\n  Token 2: World!\n";

    cr_expect_stdout_eq_str(expected_output);

    free_lexer(lexer);
    free_ast(test_ast);
}

Test(parser, command_list_bad)
{
    char *input = strdup("echo ERROR; ;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_start(lexer);

    free_lexer(lexer);

    if (!test_ast)
        cr_assert(1);
    else
    {
        free_ast(test_ast);
        cr_assert(0);
    }
}

// === Tests parsing_simple ===================================================

Test(parser, simple_cmd_simple)
{
    char *input = strdup("echo foo");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);

    cr_redirect_stdout();
    print_ast(test_ast, 0);
    fflush(NULL);

    char *expected_output =
        "SIMPLE COMMAND\n  Command:\n  Token 1: echo\n  Token 2: foo\n";

    cr_expect_stdout_eq_str(expected_output);

    free_lexer(lexer);
    free_ast(test_ast);
}

Test(parser, simple_cmd_op_bad)
{
    char *input = strdup("|| nptk");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_start(lexer);

    free_lexer(lexer);

    if (!test_ast)
        cr_assert(1);
    else
    {
        free_ast(test_ast);
        cr_assert(0);
    }
}

// === Tests parsing_if =======================================================

Test(parser, if_cmd_simple)
{
    char *input = strdup("if true; then echo ok; else echo ko; fi;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);

    cr_redirect_stdout();
    print_ast(test_ast, 0);
    fflush(NULL);

    char *expected_output =
        "IF\n  SIMPLE COMMAND\n    Command:\n    Token 1: true\nTHEN\n  SIMPLE COMMAND\n    Command:\n    Token 1: echo\n    Token 2: ok\nELSE\n  SIMPLE COMMAND\n    Command:\n    Token 1: echo\n    Token 2: ko\n";

    cr_expect_stdout_eq_str(expected_output);

    free_lexer(lexer);
    free_ast(test_ast);
}

Test(parser, if_cmd_elif)
{
    char *input = strdup("if false; then echo ko; elif true; then echo ok; else echo ko; fi;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);

    cr_redirect_stdout();
    print_ast(test_ast, 0);
    fflush(NULL);

    char *expected_output =
        "IF\n  SIMPLE COMMAND\n    Command:\n    Token 1: false\nTHEN\n  SIMPLE COMMAND\n    Command:\n    Token 1: echo\n    Token 2: ko\nELSE\n  IF\n    SIMPLE COMMAND\n      Command:\n      Token 1: true\n  THEN\n    SIMPLE COMMAND\n      Command:\n      Token 1: echo\n      Token 2: ok\n  ELSE\n    SIMPLE COMMAND\n      Command:\n      Token 1: echo\n      Token 2: ko\n";

    cr_expect_stdout_eq_str(expected_output);

    free_lexer(lexer);
    free_ast(test_ast);
}

Test(parser, if_cmd_elif_elif)
{
    char *input = strdup("if false; then echo ko; elif false; then echo ko; elif true; then echo ok; else echo ko; fi;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);

    cr_redirect_stdout();
    print_ast(test_ast, 0);
    fflush(NULL);

    char *expected_output =
        "IF\n  SIMPLE COMMAND\n    Command:\n    Token 1: false\nTHEN\n  SIMPLE COMMAND\n    Command:\n    Token 1: echo\n    Token 2: ko\nELSE\n  IF\n    SIMPLE COMMAND\n      Command:\n      Token 1: false\n  THEN\n    SIMPLE COMMAND\n      Command:\n      Token 1: echo\n      Token 2: ko\n  ELSE\n    IF\n      SIMPLE COMMAND\n        Command:\n        Token 1: true\n    THEN\n      SIMPLE COMMAND\n        Command:\n        Token 1: echo\n        Token 2: ok\n    ELSE\n      SIMPLE COMMAND\n        Command:\n        Token 1: echo\n        Token 2: ko\n";

    cr_expect_stdout_eq_str(expected_output);

    free_lexer(lexer);
    free_ast(test_ast);
}

Test(parser, if_cmd_no_else)
{
    char *input = strdup("if true; then echo ok; fi;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);

    cr_redirect_stdout();
    print_ast(test_ast, 0);
    fflush(NULL);

    char *expected_output =
        "IF\n  SIMPLE COMMAND\n    Command:\n    Token 1: true\nTHEN\n  SIMPLE COMMAND\n    Command:\n    Token 1: echo\n    Token 2: ok\nELSE\n";

    cr_expect_stdout_eq_str(expected_output);

    free_lexer(lexer);
    free_ast(test_ast);
}

Test(parser, if_cmd_elif_no_else)
{
    char *input = strdup("if false; then echo ko; elif true; then echo ok; fi;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);

    cr_redirect_stdout();
    print_ast(test_ast, 0);
    fflush(NULL);

    char *expected_output =
        "IF\n  SIMPLE COMMAND\n    Command:\n    Token 1: false\nTHEN\n  SIMPLE COMMAND\n    Command:\n    Token 1: echo\n    Token 2: ko\nELSE\n  IF\n    SIMPLE COMMAND\n      Command:\n      Token 1: true\n  THEN\n    SIMPLE COMMAND\n      Command:\n      Token 1: echo\n      Token 2: ok\n  ELSE\n";

    cr_expect_stdout_eq_str(expected_output);

    free_lexer(lexer);
    free_ast(test_ast);
}

Test(parser, if_cmd_bad_condition_1)
{
    char *input = strdup("if ; then echo ok; else echo ko; fi;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_start(lexer);

    free_lexer(lexer);

    if (!test_ast)
        cr_assert(1);
    else
    {
        free_ast(test_ast);
        cr_assert(0);
    }
}

Test(parser, if_cmd_bad_condition_2)
{
    char *input = strdup("if false; then echo ko; elif ; then echo ok; else echo ko; fi;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_start(lexer);

    free_lexer(lexer);

    if (!test_ast)
        cr_assert(1);
    else
    {
        free_ast(test_ast);
        cr_assert(0);
    }
}

Test(parser, if_cmd_bad_then_1)
{
    char *input = strdup("if true; echo ok; else echo ko; fi;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_start(lexer);

    free_lexer(lexer);

    if (!test_ast)
        cr_assert(1);
    else
    {
        free_ast(test_ast);
        cr_assert(0);
    }
}

Test(parser, if_cmd_bad_then_2)
{
    char *input = strdup("if false; then echo ko; elif true; echo ok; else echo ko; fi;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_start(lexer);

    free_lexer(lexer);

    if (!test_ast)
        cr_assert(1);
    else
    {
        free_ast(test_ast);
        cr_assert(0);
    }
}

Test(parser, if_cmd_bad_fi)
{
    char *input = strdup("if true; then echo ok; else echo ko;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_start(lexer);

    free_lexer(lexer);

    if (!test_ast)
        cr_assert(1);
    else
    {
        free_ast(test_ast);
        cr_assert(0);
    }
}

// === Tests parsing_loop =====================================================

Test(parser, for_cmd_simple)
{
    char *input = strdup("for a in seq 1 1 3; do; echo ok; done;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);

    cr_redirect_stdout();
    print_ast(test_ast, 0);
    fflush(NULL);

    char *expected_output =
        "FOR\n  Token 1: a\nIN\n  SIMPLE COMMAND\n    Command:\n    Token 1: seq\n    Token 2: 1\n    Token 3: 1\n    Token 4: 3\nDO\n  SIMPLE COMMAND\n    Command:\n    Token 1: echo\n    Token 2: ok\n";

    cr_expect_stdout_eq_str(expected_output);

    free_lexer(lexer);
    free_ast(test_ast);
}

Test(parser, until_cmd_simple)
{
    char *input = strdup("until true; do; echo ok; done;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);

    cr_redirect_stdout();
    print_ast(test_ast, 0);
    fflush(NULL);

    char *expected_output =
        "WHILE\n  SIMPLE COMMAND\n    Command:\n    Token 1: true\nDO\n  SIMPLE COMMAND\n    Command:\n    Token 1: echo\n    Token 2: ok\n";

    cr_expect_stdout_eq_str(expected_output);

    free_lexer(lexer);
    free_ast(test_ast);
}

Test(parser, while_cmd_simple)
{
    char *input = strdup("while true; do; echo ok; done;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);

    cr_redirect_stdout();
    print_ast(test_ast, 0);
    fflush(NULL);

    char *expected_output =
        "WHILE\n  SIMPLE COMMAND\n    Command:\n    Token 1: true\nDO\n  SIMPLE COMMAND\n    Command:\n    Token 1: echo\n    Token 2: ok\n";

    cr_expect_stdout_eq_str(expected_output);

    free_lexer(lexer);
    free_ast(test_ast);
}

Test(parser, for_cmd_bad_var)
{
    char *input = strdup("for in seq 1 1 3; do; echo ok; done;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_start(lexer);

    free_lexer(lexer);

    if (!test_ast)
        cr_assert(1);
    else
    {
        free_ast(test_ast);
        cr_assert(0);
    }
}

Test(parser, for_cmd_bad_in)
{
    char *input = strdup("for a seq 1 1 3; do; echo ok; done;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_start(lexer);

    free_lexer(lexer);

    if (!test_ast)
        cr_assert(1);
    else
    {
        free_ast(test_ast);
        cr_assert(0);
    }
}

Test(parser, for_cmd_bad_semicolon)
{
    char *input = strdup("for a in seq 1 1 3 do; echo ok; done;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_start(lexer);

    free_lexer(lexer);

    if (!test_ast)
        cr_assert(1);
    else
    {
        free_ast(test_ast);
        cr_assert(0);
    }
}

Test(parser, for_cmd_bad_do)
{
    char *input = strdup("for a in seq 1 1 3; echo ok; done;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_start(lexer);

    free_lexer(lexer);

    if (!test_ast)
        cr_assert(1);
    else
    {
        free_ast(test_ast);
        cr_assert(0);
    }
}

Test(parser, for_cmd_bad_done)
{
    char *input = strdup("for a in seq 1 1 3; do; echo ok;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_start(lexer);

    free_lexer(lexer);

    if (!test_ast)
        cr_assert(1);
    else
    {
        free_ast(test_ast);
        cr_assert(0);
    }
}

Test(parser, until_cmd_bad_condition)
{
    char *input = strdup("until ; do; echo ok; done;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_start(lexer);

    free_lexer(lexer);

    if (!test_ast)
        cr_assert(1);
    else
    {
        free_ast(test_ast);
        cr_assert(0);
    }
}

Test(parser, until_cmd_bad_do)
{
    char *input = strdup("until true; echo ok; done;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_start(lexer);

    free_lexer(lexer);

    if (!test_ast)
        cr_assert(1);
    else
    {
        free_ast(test_ast);
        cr_assert(0);
    }
}

Test(parser, until_cmd_bad_loop)
{
    char *input = strdup("until true; do; ; done;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_start(lexer);

    free_lexer(lexer);

    if (!test_ast)
        cr_assert(1);
    else
    {
        free_ast(test_ast);
        cr_assert(0);
    }
}

Test(parser, until_cmd_bad_done)
{
    char *input = strdup("until true; do; echo ok;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_start(lexer);

    free_lexer(lexer);

    if (!test_ast)
        cr_assert(1);
    else
    {
        free_ast(test_ast);
        cr_assert(0);
    }
}

Test(parser, while_cmd_bad_condition)
{
    char *input = strdup("while ; do; echo ok; done;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_start(lexer);

    free_lexer(lexer);

    if (!test_ast)
        cr_assert(1);
    else
    {
        free_ast(test_ast);
        cr_assert(0);
    }
}

Test(parser, while_cmd_bad_do)
{
    char *input = strdup("while true; echo ok; done;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_start(lexer);

    free_lexer(lexer);

    if (!test_ast)
        cr_assert(1);
    else
    {
        free_ast(test_ast);
        cr_assert(0);
    }
}

Test(parser, while_cmd_bad_loop)
{
    char *input = strdup("while true; do; ; done;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_start(lexer);

    free_lexer(lexer);

    if (!test_ast)
        cr_assert(1);
    else
    {
        free_ast(test_ast);
        cr_assert(0);
    }
}

Test(parser, while_cmd_bad_done)
{
    char *input = strdup("while true; do; echo ok;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_start(lexer);

    free_lexer(lexer);

    if (!test_ast)
        cr_assert(1);
    else
    {
        free_ast(test_ast);
        cr_assert(0);
    }
}

// === Tests parsing_ops ======================================================

Test(parser, pipelines_cmd_simple)
{
    char *input = strdup("echo ok | cat -e");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);

    cr_redirect_stdout();
    print_ast(test_ast, 0);
    fflush(NULL);

    char *expected_output =
        "PIPELINES\nleft  SIMPLE COMMAND\n    Command:\n    Token 1: echo\n    Token 2: ok\nright  SIMPLE COMMAND\n    Command:\n    Token 1: cat\n    Token 2: -e\n";

    cr_expect_stdout_eq_str(expected_output);

    free_lexer(lexer);
    free_ast(test_ast);
}

Test(parser, pipelines_cmd_bad_right)
{
    char *input = strdup("echo ok | ;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_start(lexer);

    free_lexer(lexer);

    if (!test_ast)
        cr_assert(1);
    else
    {
        free_ast(test_ast);
        cr_assert(0);
    }
}

Test(parser, neg_pipelines_cmd_simple)
{
    char *input = strdup("! echo ok | cat -e");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);

    cr_redirect_stdout();
    print_ast(test_ast, 0);
    fflush(NULL);

    char *expected_output =
        "NEG\n  PIPELINES\n  left    SIMPLE COMMAND\n      Command:\n      Token 1: echo\n      Token 2: ok\n  right    SIMPLE COMMAND\n      Command:\n      Token 1: cat\n      Token 2: -e\n";

    cr_expect_stdout_eq_str(expected_output);

    free_lexer(lexer);
    free_ast(test_ast);
}

Test(parser, neg_pipelines_cmd_bad)
{
    char *input = strdup("! echo ok | ;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_start(lexer);

    free_lexer(lexer);

    if (!test_ast)
        cr_assert(1);
    else
    {
        free_ast(test_ast);
        cr_assert(0);
    }
}

Test(parser, and_cmd_simple)
{
    char *input = strdup("echo ok && echo OK");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);

    cr_redirect_stdout();
    print_ast(test_ast, 0);
    fflush(NULL);

    char *expected_output =
        "AND\nleft  SIMPLE COMMAND\n    Command:\n    Token 1: echo\n    Token 2: ok\nright  SIMPLE COMMAND\n    Command:\n    Token 1: echo\n    Token 2: OK\n";

    cr_expect_stdout_eq_str(expected_output);

    free_lexer(lexer);
    free_ast(test_ast);
}

Test(parser, or_cmd_simple)
{
    char *input = strdup("echo ok || echo OK");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);

    cr_redirect_stdout();
    print_ast(test_ast, 0);
    fflush(NULL);

    char *expected_output =
        "OR\nleft  SIMPLE COMMAND\n    Command:\n    Token 1: echo\n    Token 2: ok\nright  SIMPLE COMMAND\n    Command:\n    Token 1: echo\n    Token 2: OK\n";

    cr_expect_stdout_eq_str(expected_output);

    free_lexer(lexer);
    free_ast(test_ast);
}

Test(parser, and_cmd_bad)
{
    char *input = strdup("echo ok && ;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_start(lexer);

    free_lexer(lexer);

    if (!test_ast)
        cr_assert(1);
    else
    {
        free_ast(test_ast);
        cr_assert(0);
    }
}

Test(parser, or_cmd_bad)
{
    char *input = strdup("echo ok || ;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_start(lexer);

    free_lexer(lexer);

    if (!test_ast)
        cr_assert(1);
    else
    {
        free_ast(test_ast);
        cr_assert(0);
    }
}

// === Tests print_ast ========================================================

Test(parser, print_single_quotes)
{
    char *input = strdup("echo 'Hello World!';");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);

    cr_redirect_stdout();
    print_ast(test_ast, 0);
    fflush(NULL);

    char *expected_output =
        "SIMPLE COMMAND\n  Command:\n  Token 1: echo\n  Token 2: 'Hello World!'\n";

    cr_expect_stdout_eq_str(expected_output);

    free_lexer(lexer);
    free_ast(test_ast);
}

Test(parser, print_double_quotes)
{
    char *input = strdup("echo \"Hello World!\";");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);

    cr_redirect_stdout();
    print_ast(test_ast, 0);
    fflush(NULL);

    char *expected_output =
        "SIMPLE COMMAND\n  Command:\n  Token 1: echo\n  Token 2: \"Hello World!\"\n";

    cr_expect_stdout_eq_str(expected_output);

    free_lexer(lexer);
    free_ast(test_ast);
}
