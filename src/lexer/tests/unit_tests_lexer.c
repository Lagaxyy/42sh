#ifndef _POSIX_C_SOURCE
#    define _POSIX_C_SOURCE 200809L
#endif /* ! _POSIX_C_SOURCE */

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdlib.h>
#include <string.h>

#include "../lexer.h"

TestSuite(lexer);

Test(lexer, dummy)
{
    cr_assert(1);
}

// === Tests coverage free_lexer ==============================================

Test(lexer, free_null)
{
    free_lexer(NULL);
    cr_assert(1);
}

// === Tests coverage lexerVerif ==============================================

Test(lexer, lexerVerif_word)
{
    char *input = strdup("Test");
    struct Lexer *test_lexer = init_lexer(input);
    int check = lexerVerif(test_lexer, WORD);

    int expected_check = 0; // false

    cr_expect_eq(check, expected_check);

    free_lexer(test_lexer);
}

Test(lexer, lexerVerif_if)
{
    char *input = strdup("if");
    struct Lexer *test_lexer = init_lexer(input);
    int check = lexerVerif(test_lexer, IF_GRM);

    int expected_check = 1; // true

    cr_expect_eq(check, expected_check);

    free_lexer(test_lexer);
}

Test(lexer, lexerVerif_then)
{
    char *input = strdup("then");
    struct Lexer *test_lexer = init_lexer(input);
    int check = lexerVerif(test_lexer, THEN_GRM);

    int expected_check = 1; // true

    cr_expect_eq(check, expected_check);

    free_lexer(test_lexer);
}

Test(lexer, lexerVerif_else)
{
    char *input = strdup("else");
    struct Lexer *test_lexer = init_lexer(input);
    int check = lexerVerif(test_lexer, ELSE_GRM);

    int expected_check = 1; // true

    cr_expect_eq(check, expected_check);

    free_lexer(test_lexer);
}

Test(lexer, lexerVerif_elif)
{
    char *input = strdup("elif");
    struct Lexer *test_lexer = init_lexer(input);
    int check = lexerVerif(test_lexer, ELIF_GRM);

    int expected_check = 1; // true

    cr_expect_eq(check, expected_check);

    free_lexer(test_lexer);
}

Test(lexer, lexerVerif_fi)
{
    char *input = strdup("fi");
    struct Lexer *test_lexer = init_lexer(input);
    int check = lexerVerif(test_lexer, FI_GRM);

    int expected_check = 1; // true

    cr_expect_eq(check, expected_check);

    free_lexer(test_lexer);
}

Test(lexer, lexerVerif_while)
{
    char *input = strdup("while");
    struct Lexer *test_lexer = init_lexer(input);
    int check = lexerVerif(test_lexer, WHILE_GRM);

    int expected_check = 1; // true

    cr_expect_eq(check, expected_check);

    free_lexer(test_lexer);
}

Test(lexer, lexerVerif_until)
{
    char *input = strdup("until");
    struct Lexer *test_lexer = init_lexer(input);
    int check = lexerVerif(test_lexer, UNTIL_GRM);

    int expected_check = 1; // true

    cr_expect_eq(check, expected_check);

    free_lexer(test_lexer);
}

Test(lexer, lexerVerif_do)
{
    char *input = strdup("do");
    struct Lexer *test_lexer = init_lexer(input);
    int check = lexerVerif(test_lexer, DO_GRM);

    int expected_check = 1; // true

    cr_expect_eq(check, expected_check);

    free_lexer(test_lexer);
}

Test(lexer, lexerVerif_done)
{
    char *input = strdup("done");
    struct Lexer *test_lexer = init_lexer(input);
    int check = lexerVerif(test_lexer, DONE_GRM);

    int expected_check = 1; // true

    cr_expect_eq(check, expected_check);

    free_lexer(test_lexer);
}

Test(lexer, lexerVerif_semicolon)
{
    char *input = strdup(";");
    struct Lexer *test_lexer = init_lexer(input);
    int check = lexerVerif(test_lexer, SEMICOLON);

    int expected_check = 1; // true

    cr_expect_eq(check, expected_check);

    free_lexer(test_lexer);
}

Test(lexer, lexerVerif_pipelines)
{
    char *input = strdup("|");
    struct Lexer *test_lexer = init_lexer(input);
    int check = lexerVerif(test_lexer, PIPELINES);

    int expected_check = 1; // true

    cr_expect_eq(check, expected_check);

    free_lexer(test_lexer);
}

Test(lexer, lexerVerif_and)
{
    char *input = strdup("&&");
    struct Lexer *test_lexer = init_lexer(input);
    int check = lexerVerif(test_lexer, AND_GRM);

    int expected_check = 1; // true

    cr_expect_eq(check, expected_check);

    free_lexer(test_lexer);
}

Test(lexer, lexerVerif_or)
{
    char *input = strdup("||");
    struct Lexer *test_lexer = init_lexer(input);
    int check = lexerVerif(test_lexer, OR_GRM);

    int expected_check = 1; // true

    cr_expect_eq(check, expected_check);

    free_lexer(test_lexer);
}

Test(lexer, lexerVerif_true_end)
{
    char *input = strdup("");
    struct Lexer *test_lexer = init_lexer(input);
    int check = lexerVerif(test_lexer, END);

    int expected_check = 1; // true

    cr_expect_eq(check, expected_check);

    free_lexer(test_lexer);
}

Test(lexer, lexerVerif_false_end)
{
    char *input = strdup("");
    struct Lexer *test_lexer = init_lexer(input);
    int check = lexerVerif(test_lexer, WORD);

    int expected_check = 0; // false

    cr_expect_eq(check, expected_check);

    free_lexer(test_lexer);
}

// === Tests coverage lexer_check =============================================

Test(lexer, lexer_check_false)
{
    char *input = strdup("Test");
    struct Lexer *test_lexer = init_lexer(input);
    bool check = lexer_check(test_lexer);

    bool expected_check = false;

    cr_expect_eq(check, expected_check);

    free_lexer(test_lexer);
}

Test(lexer, lexer_check_then)
{
    char *input = strdup("then");
    struct Lexer *test_lexer = init_lexer(input);
    bool check = lexer_check(test_lexer);

    bool expected_check = true;

    cr_expect_eq(check, expected_check);

    free_lexer(test_lexer);
}

Test(lexer, lexer_check_elif)
{
    char *input = strdup("elif");
    struct Lexer *test_lexer = init_lexer(input);
    bool check = lexer_check(test_lexer);

    bool expected_check = true;

    cr_expect_eq(check, expected_check);

    free_lexer(test_lexer);
}

Test(lexer, lexer_check_else)
{
    char *input = strdup("else");
    struct Lexer *test_lexer = init_lexer(input);
    bool check = lexer_check(test_lexer);

    bool expected_check = true;

    cr_expect_eq(check, expected_check);

    free_lexer(test_lexer);
}

Test(lexer, lexer_check_fi)
{
    char *input = strdup("fi");
    struct Lexer *test_lexer = init_lexer(input);
    bool check = lexer_check(test_lexer);

    bool expected_check = true;

    cr_expect_eq(check, expected_check);

    free_lexer(test_lexer);
}

Test(lexer, lexer_check_do)
{
    char *input = strdup("do");
    struct Lexer *test_lexer = init_lexer(input);
    bool check = lexer_check(test_lexer);

    bool expected_check = true;

    cr_expect_eq(check, expected_check);

    free_lexer(test_lexer);
}

Test(lexer, lexer_check_done)
{
    char *input = strdup("done");
    struct Lexer *test_lexer = init_lexer(input);
    bool check = lexer_check(test_lexer);

    bool expected_check = true;

    cr_expect_eq(check, expected_check);

    free_lexer(test_lexer);
}

// === Tests coverage lexer ===================================================

Test(lexer, end)
{
    char *input = strdup("Test");
    struct Lexer *test_lexer = init_lexer(input);
    struct Token *test_token = lexer(test_lexer, END);

    free_lexer(test_lexer);

    if (!test_token)
        cr_assert(1);
    else
    {
        free(test_token);
        cr_assert(0);
    }
}

Test(lexer, op_and)
{
    char *input = strdup("&&");
    struct Lexer *test_lexer = init_lexer(input);
    struct Token *test_token = lexer(test_lexer, WORD);

    free_lexer(test_lexer);

    if (!test_token)
        cr_assert(1);
    else
    {
        free(test_token);
        cr_assert(0);
    }
}

Test(lexer, op_pipe)
{
    char *input = strdup("|");
    struct Lexer *test_lexer = init_lexer(input);
    struct Token *test_token = lexer(test_lexer, WORD);

    free_lexer(test_lexer);

    if (!test_token)
        cr_assert(1);
    else
    {
        free(test_token);
        cr_assert(0);
    }
}

Test(lexer, op_or)
{
    char *input = strdup("||");
    struct Lexer *test_lexer = init_lexer(input);
    struct Token *test_token = lexer(test_lexer, WORD);

    free_lexer(test_lexer);

    if (!test_token)
        cr_assert(1);
    else
    {
        free(test_token);
        cr_assert(0);
    }
}

Test(lexer, op_negation)
{
    char *input = strdup("!");
    struct Lexer *test_lexer = init_lexer(input);
    struct Token *test_token = lexer(test_lexer, WORD);

    free_lexer(test_lexer);

    if (!test_token)
        cr_assert(1);
    else
    {
        free(test_token);
        cr_assert(0);
    }
}

Test(lexer, single_quotes)
{
    char *input = strdup("'Test'");
    struct Lexer *test_lexer = init_lexer(input);
    struct Token *test_token = lexer(test_lexer, WORD);

    size_t expected_len = 4;
    char *expected_str = "Test";

    cr_expect_eq(test_token->len, expected_len, "Expected: %d. Got: %d.",
                 expected_len, test_token->len);
    for (size_t i = 0; i < test_token->len; i++)
        cr_expect_eq(test_token->str[i], expected_str[i],
                     "Expected: %c. Got: %c.", expected_str[i],
                     test_token->str[i]);

    free(test_token);
    free_lexer(test_lexer);
}

Test(lexer, bad_single_quotes)
{
    char *input = strdup("'Test");
    struct Lexer *test_lexer = init_lexer(input);
    struct Token *test_token = lexer(test_lexer, WORD);

    free_lexer(test_lexer);

    if (!test_token)
        cr_assert(1);
    else
    {
        free(test_token);
        cr_assert(0);
    }
}

Test(lexer, comment)
{
    char *input = strdup("# Test");
    struct Lexer *test_lexer = init_lexer(input);
    struct Token *test_token = lexer(test_lexer, WORD);

    free_lexer(test_lexer);

    if (!test_token)
        cr_assert(1);
    else
    {
        free(test_token);
        cr_assert(0);
    }
}

// === Tests classiques =========================================================

Test(lexer, one_word)
{
    char *input = strdup("Test");
    struct Lexer *test_lexer = init_lexer(input);
    struct Token *test_token = lexer(test_lexer, WORD);

    size_t expected_len = 4;
    char *expected_str = "Test";

    cr_expect_eq(test_token->len, expected_len, "Expected: %d. Got: %d.",
                 expected_len, test_token->len);
    for (size_t i = 0; i < test_token->len; i++)
        cr_expect_eq(test_token->str[i], expected_str[i],
                     "Expected: %c. Got: %c.", expected_str[i],
                     test_token->str[i]);

    free(test_token);
    free_lexer(test_lexer);
}

Test(lexer, many_words)
{
    char *input = strdup("if echo foo then echo bar else echo baz fi");
    char *expected_strlist[] = { "if",  "echo", "foo",  "then", "echo",
                                 "bar", "else", "echo", "baz",  "fi" };
    size_t size = 10;
    size_t i = 0;

    struct Lexer *test_lexer = init_lexer(input);
    struct Token *test_token = NULL;

    while (1)
    {
        test_token = lexer(test_lexer, WORD);
        if (!test_token || i > size + 1)
            break;

        char *expected_str = expected_strlist[i];
        size_t expected_len = strlen(expected_str);

        cr_expect_eq(test_token->len, expected_len, "Expected: %d. Got: %d.",
                     expected_len, test_token->len);
        for (size_t j = 0; j < test_token->len; j++)
            cr_expect_eq(test_token->str[j], expected_str[j],
                         "Expected: %c. Got: %c.", expected_str[j],
                         test_token->str[j]);

        input = test_token->str + test_token->len;

        free(test_token);
        i++;
    }

    cr_expect_eq(i, size, "Expected: %d. Got: %d.", size, i);
    free_lexer(test_lexer);
}

Test(lexer, blank_first)
{
    char *input = strdup("   \t \t\t echo 52 1;");
    char *expected_strlist[] = { "echo", "52", "1" };
    size_t size = 3;
    size_t i = 0;

    struct Lexer *test_lexer = init_lexer(input);
    struct Token *test_token = NULL;

    while (1)
    {
        test_token = lexer(test_lexer, WORD);
        if (!test_token || i > size + 1)
            break;

        char *expected_str = expected_strlist[i];
        size_t expected_len = strlen(expected_str);

        cr_expect_eq(test_token->len, expected_len, "Expected: %d. Got: %d.",
                     expected_len, test_token->len);
        for (size_t j = 0; j < test_token->len; j++)
            cr_expect_eq(test_token->str[j], expected_str[j],
                         "Expected: %c. Got: %c.", expected_str[j],
                         test_token->str[j]);

        input = test_token->str + test_token->len;

        free(test_token);
        i++;
    }

    cr_expect_eq(i, size, "Expected: %d. Got: %d.", size, i);
    free_lexer(test_lexer);
}
