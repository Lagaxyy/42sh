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
    int end_cmd = 0;

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
    int end_cmd = 0;

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
