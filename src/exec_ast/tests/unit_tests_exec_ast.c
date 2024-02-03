#ifndef _POSIX_C_SOURCE
#    define _POSIX_C_SOURCE 200809L
#endif /* ! _POSIX_C_SOURCE */

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../lexer/lexer.h"
#include "../../parser/parsing.h"
#include "../exec_ast.h"

// Coverage: 91.5% (03/02/2024)
TestSuite(exec_ast);

Test(exec_ast, dummy)
{
    cr_assert(1);
}

// === Tests builtin_echo =====================================================

Test(exec_ast, echo_simple_1)
{
    char *input = strdup("echo Hello World!;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);

    cr_redirect_stdout();
    exec_ast_init(test_ast);
    fflush(NULL);

    char *expected_output = "Hello World!\n";

    cr_expect_stdout_eq_str(expected_output);

    free_lexer(lexer);
    free_ast(test_ast);
}

Test(exec_ast, echo_simple_2)
{
    char *input = strdup("echo Hello World! \\\\n;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);

    cr_redirect_stdout();
    exec_ast_init(test_ast);
    fflush(NULL);

    char *expected_output = "Hello World! \\n\n";

    cr_expect_stdout_eq_str(expected_output);

    free_lexer(lexer);
    free_ast(test_ast);
}

Test(exec_ast, echo_n_option)
{
    char *input = strdup("echo -n Hello World! \\n");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);

    cr_redirect_stdout();
    exec_ast_init(test_ast);
    fflush(NULL);

    char *expected_output = "Hello World! n";

    cr_expect_stdout_eq_str(expected_output);

    free_lexer(lexer);
    free_ast(test_ast);
}

Test(exec_ast, echo_e_option_1)
{
    char *input = strdup("echo -e Hello World! \\n");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);

    cr_redirect_stdout();
    exec_ast_init(test_ast);
    fflush(NULL);

    char *expected_output = "Hello World! n\n";

    cr_expect_stdout_eq_str(expected_output);

    free_lexer(lexer);
    free_ast(test_ast);
}

Test(exec_ast, echo_e_option_2)
{
    char *input = strdup("echo -e Hello World! \\\\n;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);

    cr_redirect_stdout();
    exec_ast_init(test_ast);
    fflush(NULL);

    char *expected_output = "Hello World! \n\n";

    cr_expect_stdout_eq_str(expected_output);

    free_lexer(lexer);
    free_ast(test_ast);
}

Test(exec_ast, echo_e_option_3)
{
    char *input = strdup("echo -e Hello World! \\\\t;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);

    cr_redirect_stdout();
    exec_ast_init(test_ast);
    fflush(NULL);

    char *expected_output = "Hello World! \t\n";

    cr_expect_stdout_eq_str(expected_output);

    free_lexer(lexer);
    free_ast(test_ast);
}

Test(exec_ast, echo_e_option_4)
{
    char *input = strdup("echo -e Hello World! \\\\\\;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);

    cr_redirect_stdout();
    exec_ast_init(test_ast);
    fflush(NULL);

    char *expected_output = "Hello World! \\\n";

    cr_expect_stdout_eq_str(expected_output);

    free_lexer(lexer);
    free_ast(test_ast);
}

Test(exec_ast, echo_E_option)
{
    char *input = strdup("echo -E Hello World! \\n");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);

    cr_redirect_stdout();
    exec_ast_init(test_ast);
    fflush(NULL);

    char *expected_output = "Hello World! n\n";

    cr_expect_stdout_eq_str(expected_output);

    free_lexer(lexer);
    free_ast(test_ast);
}

Test(exec_ast, echo_bad_option)
{
    char *input = strdup("echo -nez Hello World!;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);

    cr_redirect_stdout();
    exec_ast_init(test_ast);
    fflush(NULL);

    char *expected_output = "-nez Hello World!\n";

    cr_expect_stdout_eq_str(expected_output);

    free_lexer(lexer);
    free_ast(test_ast);
}

Test(exec_ast, echo_no_args)
{
    char *input = strdup("echo ;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);

    cr_redirect_stdout();
    exec_ast_init(test_ast);
    fflush(NULL);

    char *expected_output = "\n";

    cr_expect_stdout_eq_str(expected_output);

    free_lexer(lexer);
    free_ast(test_ast);
}

Test(exec_ast, echo_inside_echo)
{
    char *input = strdup("echo Hello /bin/echo World!;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);

    cr_redirect_stdout();
    exec_ast_init(test_ast);
    fflush(NULL);

    char *expected_output = "Hello\nWorld!\n";

    cr_expect_stdout_eq_str(expected_output);

    free_lexer(lexer);
    free_ast(test_ast);
}

// === Tests run_cmds =========================================================

Test(exec_ast, simple_cmd_non_builtin)
{
    char *input = strdup("ls /bin;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);
    cr_redirect_stdout();
    int exitcode = exec_ast_init(test_ast);
    fflush(NULL);
    free_lexer(lexer);
    free_ast(test_ast);

    // Test seulement pour savoir si tout run bien
    // Creer un dossier temporaire pour check l'output
    // de la commande ?
    cr_expect_eq(exitcode, 0, "Expected: %d. Got: %d.", 0, exitcode);
}

/* Run un nouveau programme, donc risque de false leaks
Test(exec_ast, dot)
{
    char *input = strdup(". -c \"echo foo\";");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);
    cr_redirect_stdout();
    int exitcode = exec_ast_init(test_ast);
    fflush(NULL);
    free_lexer(lexer);
    free_ast(test_ast);

    // Test seulement pour savoir si tout run bien
    // Creer un dossier temporaire pour check l'output
    // de la commande ?
    cr_expect_eq(exitcode, 127, "Expected: %d. Got: %d.", 127, exitcode);
}
*/

// === Tests merge_token et list_variable =====================================

Test(exec_ast, merge_token_single_quotes)
{
    char *input = strdup("echo 'Hello World!';");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);
    cr_redirect_stdout();
    int exitcode = exec_ast_init(test_ast);
    fflush(NULL);
    free_lexer(lexer);
    free_ast(test_ast);

    // Test seulement pour savoir si tout run bien
    // Creer un dossier temporaire pour check l'output
    // de la commande ?
    cr_expect_eq(exitcode, 0, "Expected: %d. Got: %d.", 0, exitcode);
}

Test(exec_ast, merge_token_double_quotes)
{
    char *input = strdup("echo \"Hello World!\";");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);
    cr_redirect_stdout();
    int exitcode = exec_ast_init(test_ast);
    fflush(NULL);
    free_lexer(lexer);
    free_ast(test_ast);

    // Test seulement pour savoir si tout run bien
    // Creer un dossier temporaire pour check l'output
    // de la commande ?
    cr_expect_eq(exitcode, 0, "Expected: %d. Got: %d.", 0, exitcode);
}

Test(exec_ast, define_variable)
{
    char *input = strdup("a=Hello");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);
    cr_redirect_stdout();
    int exitcode = exec_ast_init(test_ast);
    fflush(NULL);
    free_lexer(lexer);
    free_ast(test_ast);

    // Test seulement pour savoir si tout run bien
    // Creer un dossier temporaire pour check l'output
    // de la commande ?
    cr_expect_eq(exitcode, 0, "Expected: %d. Got: %d.", 0, exitcode);
}

Test(exec_ast, define_variable_and_echo_1)
{
    char *input = strdup("a=Hello; echo $a;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);
    cr_redirect_stdout();
    int exitcode = exec_ast_init(test_ast);
    fflush(NULL);
    free_lexer(lexer);
    free_ast(test_ast);

    // Test seulement pour savoir si tout run bien
    // Creer un dossier temporaire pour check l'output
    // de la commande ?
    cr_expect_eq(exitcode, 0, "Expected: %d. Got: %d.", 0, exitcode);
}

Test(exec_ast, define_variable_and_echo_2)
{
    char *input = strdup("a='Hello World'; echo $a is a famous sentence;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);
    cr_redirect_stdout();
    int exitcode = exec_ast_init(test_ast);
    fflush(NULL);
    free_lexer(lexer);
    free_ast(test_ast);

    // Test seulement pour savoir si tout run bien
    // Creer un dossier temporaire pour check l'output
    // de la commande ?
    cr_expect_eq(exitcode, 0, "Expected: %d. Got: %d.", 0, exitcode);
}

Test(exec_ast, define_variable_and_echo_double_quotes)
{
    char *input = strdup("a=Hello; echo \"$a\";");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);
    cr_redirect_stdout();
    int exitcode = exec_ast_init(test_ast);
    fflush(NULL);
    free_lexer(lexer);
    free_ast(test_ast);

    // Test seulement pour savoir si tout run bien
    // Creer un dossier temporaire pour check l'output
    // de la commande ?
    cr_expect_eq(exitcode, 0, "Expected: %d. Got: %d.", 0, exitcode);
}

Test(exec_ast, define_variable_single_quotes_and_echo_double_quotes)
{
    char *input = strdup("a='Hello'; echo \"$a\";");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);
    cr_redirect_stdout();
    int exitcode = exec_ast_init(test_ast);
    fflush(NULL);
    free_lexer(lexer);
    free_ast(test_ast);

    // Test seulement pour savoir si tout run bien
    // Creer un dossier temporaire pour check l'output
    // de la commande ?
    cr_expect_eq(exitcode, 0, "Expected: %d. Got: %d.", 0, exitcode);
}

Test(exec_ast, define_variable_and_echo_single_quotes)
{
    char *input = strdup("a=Hello; echo '$a';");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);
    cr_redirect_stdout();
    int exitcode = exec_ast_init(test_ast);
    fflush(NULL);
    free_lexer(lexer);
    free_ast(test_ast);

    // Test seulement pour savoir si tout run bien
    // Creer un dossier temporaire pour check l'output
    // de la commande ?
    cr_expect_eq(exitcode, 0, "Expected: %d. Got: %d.", 0, exitcode);
}

Test(exec_ast, define_many_variables_1)
{
    char *input = strdup("a=Hello; b=World;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);
    cr_redirect_stdout();
    int exitcode = exec_ast_init(test_ast);
    fflush(NULL);
    free_lexer(lexer);
    free_ast(test_ast);

    // Test seulement pour savoir si tout run bien
    // Creer un dossier temporaire pour check l'output
    // de la commande ?
    cr_expect_eq(exitcode, 0, "Expected: %d. Got: %d.", 0, exitcode);
}

Test(exec_ast, define_many_variables_2)
{
    char *input = strdup("a=Hello; b=World; c=Test.; d=OK; a=Goodbye; c=KO;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);
    cr_redirect_stdout();
    int exitcode = exec_ast_init(test_ast);
    fflush(NULL);
    free_lexer(lexer);
    free_ast(test_ast);

    // Test seulement pour savoir si tout run bien
    // Creer un dossier temporaire pour check l'output
    // de la commande ?
    cr_expect_eq(exitcode, 0, "Expected: %d. Got: %d.", 0, exitcode);
}

// === Tests exec_ast =========================================================

Test(exec_ast, builtin_true)
{
    char *input = strdup("true;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);
    cr_redirect_stdout();
    int exitcode = exec_ast_init(test_ast);
    fflush(NULL);
    free_lexer(lexer);
    free_ast(test_ast);

    // Test seulement pour savoir si tout run bien
    // Creer un dossier temporaire pour check l'output
    // de la commande ?
    cr_expect_eq(exitcode, 0, "Expected: %d. Got: %d.", 0, exitcode);
}

Test(exec_ast, builtin_false)
{
    char *input = strdup("false;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);
    cr_redirect_stdout();
    int exitcode = exec_ast_init(test_ast);
    fflush(NULL);
    free_lexer(lexer);
    free_ast(test_ast);

    // Test seulement pour savoir si tout run bien
    // Creer un dossier temporaire pour check l'output
    // de la commande ?
    cr_expect_eq(exitcode, 1, "Expected: %d. Got: %d.", 1, exitcode);
}

Test(exec_ast, builtin_exit0)
{
    char *input = strdup("exit 0;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);
    cr_redirect_stdout();
    int exitcode = exec_ast_init(test_ast);
    fflush(NULL);
    free_lexer(lexer);
    free_ast(test_ast);

    // Test seulement pour savoir si tout run bien
    // Creer un dossier temporaire pour check l'output
    // de la commande ?
    cr_expect_eq(exitcode, 0, "Expected: %d. Got: %d.", 0, exitcode);
}

Test(exec_ast, builtin_continue)
{
    char *input = strdup("continue;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);
    cr_redirect_stdout();
    int exitcode = exec_ast_init(test_ast);
    fflush(NULL);
    free_lexer(lexer);
    free_ast(test_ast);

    // Test seulement pour savoir si tout run bien
    // Creer un dossier temporaire pour check l'output
    // de la commande ?
    cr_expect_eq(exitcode, 0, "Expected: %d. Got: %d.", 0, exitcode);
}

Test(exec_ast, builtin_break)
{
    char *input = strdup("break");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);
    cr_redirect_stdout();
    int exitcode = exec_ast_init(test_ast);
    fflush(NULL);
    free_lexer(lexer);
    free_ast(test_ast);

    // Test seulement pour savoir si tout run bien
    // Creer un dossier temporaire pour check l'output
    // de la commande ?
    cr_expect_eq(exitcode, 0, "Expected: %d. Got: %d.", 0, exitcode);
}

Test(exec_ast, if_cmd_1)
{
    char *input = strdup("if true; then echo ok; else echo ko; fi;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);
    cr_redirect_stdout();
    int exitcode = exec_ast_init(test_ast);
    fflush(NULL);
    free_lexer(lexer);
    free_ast(test_ast);

    // Test seulement pour savoir si tout run bien
    // Creer un dossier temporaire pour check l'output
    // de la commande ?
    cr_expect_eq(exitcode, 0, "Expected: %d. Got: %d.", 0, exitcode);
}

Test(exec_ast, if_cmd_2)
{
    char *input = strdup("if false; then echo ko; else echo ok; fi;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);
    cr_redirect_stdout();
    int exitcode = exec_ast_init(test_ast);
    fflush(NULL);
    free_lexer(lexer);
    free_ast(test_ast);

    // Test seulement pour savoir si tout run bien
    // Creer un dossier temporaire pour check l'output
    // de la commande ?
    cr_expect_eq(exitcode, 0, "Expected: %d. Got: %d.", 0, exitcode);
}

/*
Test(exec_ast, pipeline_cmd)
{
    char *input = strdup("echo ok | cat -e");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);
    int exitcode = exec_ast_init(test_ast);
    free_lexer(lexer);
    free_ast(test_ast);

    // Test seulement pour savoir si tout run bien
    // Creer un dossier temporaire pour check l'output
    // de la commande ?
    cr_expect_eq(exitcode, 0, "Expected: %d. Got: %d.", 0, exitcode);
}

Test(exec_ast, neg_cmd)
{
    char *input = strdup("! echo ok | cat -e");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);
    cr_redirect_stdout();
    int exitcode = exec_ast_init(test_ast);
    fflush(NULL);
    free_lexer(lexer);
    free_ast(test_ast);

    // Test seulement pour savoir si tout run bien
    // Creer un dossier temporaire pour check l'output
    // de la commande ?
    cr_expect_eq(exitcode, 1, "Expected: %d. Got: %d.", 1, exitcode);
}
*/

Test(exec_ast, while_cmd)
{
    char *input = strdup("while true; do; echo ok; break; done;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);
    cr_redirect_stdout();
    int exitcode = exec_ast_init(test_ast);
    fflush(NULL);
    free_lexer(lexer);
    free_ast(test_ast);

    // Test seulement pour savoir si tout run bien
    // Creer un dossier temporaire pour check l'output
    // de la commande ?
    cr_expect_eq(exitcode, 0, "Expected: %d. Got: %d.", 0, exitcode);
}

Test(exec_ast, until_cmd)
{
    char *input = strdup("until false; do; echo ok; break; done;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);
    cr_redirect_stdout();
    int exitcode = exec_ast_init(test_ast);
    fflush(NULL);
    free_lexer(lexer);
    free_ast(test_ast);

    // Test seulement pour savoir si tout run bien
    // Creer un dossier temporaire pour check l'output
    // de la commande ?
    cr_expect_eq(exitcode, 0, "Expected: %d. Got: %d.", 0, exitcode);
}

Test(exec_ast, and_cmd_1)
{
    char *input = strdup("echo Hello && echo World");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);
    cr_redirect_stdout();
    int exitcode = exec_ast_init(test_ast);
    fflush(NULL);
    free_lexer(lexer);
    free_ast(test_ast);

    // Test seulement pour savoir si tout run bien
    // Creer un dossier temporaire pour check l'output
    // de la commande ?
    cr_expect_eq(exitcode, 0, "Expected: %d. Got: %d.", 0, exitcode);
}

Test(exec_ast, and_cmd_2)
{
    char *input = strdup("false && echo never");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);
    cr_redirect_stdout();
    int exitcode = exec_ast_init(test_ast);
    fflush(NULL);
    free_lexer(lexer);
    free_ast(test_ast);

    // Test seulement pour savoir si tout run bien
    // Creer un dossier temporaire pour check l'output
    // de la commande ?
    cr_expect_eq(exitcode, 1, "Expected: %d. Got: %d.", 1, exitcode);
}

Test(exec_ast, or_cmd_1)
{
    char *input = strdup("echo Hello || echo World");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);
    cr_redirect_stdout();
    int exitcode = exec_ast_init(test_ast);
    fflush(NULL);
    free_lexer(lexer);
    free_ast(test_ast);

    // Test seulement pour savoir si tout run bien
    // Creer un dossier temporaire pour check l'output
    // de la commande ?
    cr_expect_eq(exitcode, 0, "Expected: %d. Got: %d.", 0, exitcode);
}

Test(exec_ast, or_cmd_2)
{
    char *input = strdup("false || echo Hello World!");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);
    cr_redirect_stdout();
    int exitcode = exec_ast_init(test_ast);
    fflush(NULL);
    free_lexer(lexer);
    free_ast(test_ast);

    // Test seulement pour savoir si tout run bien
    // Creer un dossier temporaire pour check l'output
    // de la commande ?
    cr_expect_eq(exitcode, 0, "Expected: %d. Got: %d.", 0, exitcode);
}

Test(exec_ast, for_cmd)
{
    char *input = strdup("for a in seq 1 1 3; do; echo $a; done;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);
    cr_redirect_stdout();
    int exitcode = exec_ast_init(test_ast);
    fflush(NULL);
    free_lexer(lexer);
    free_ast(test_ast);

    // Test seulement pour savoir si tout run bien
    // Creer un dossier temporaire pour check l'output
    // de la commande ?
    cr_expect_eq(exitcode, 0, "Expected: %d. Got: %d.", 0, exitcode);
}

Test(exec_ast, for_cmd_continue)
{
    char *input = strdup("for a in seq 1 1 3; do; continue; done;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);
    cr_redirect_stdout();
    int exitcode = exec_ast_init(test_ast);
    fflush(NULL);
    free_lexer(lexer);
    free_ast(test_ast);

    // Test seulement pour savoir si tout run bien
    // Creer un dossier temporaire pour check l'output
    // de la commande ?
    cr_expect_eq(exitcode, 0, "Expected: %d. Got: %d.", 0, exitcode);
}

Test(exec_ast, for_cmd_break)
{
    char *input = strdup("for a in seq 1 1 3; do; break; done;");
    struct Lexer *lexer = init_lexer(input);
    struct Ast *test_ast = parse_command(lexer);
    cr_redirect_stdout();
    int exitcode = exec_ast_init(test_ast);
    fflush(NULL);
    free_lexer(lexer);
    free_ast(test_ast);

    // Test seulement pour savoir si tout run bien
    // Creer un dossier temporaire pour check l'output
    // de la commande ?
    cr_expect_eq(exitcode, 0, "Expected: %d. Got: %d.", 0, exitcode);
}

Test(exec_ast, null)
{
    cr_redirect_stdout();
    int exitcode = exec_ast_init(NULL);
    fflush(NULL);

    // Test seulement pour savoir si tout run bien
    // Creer un dossier temporaire pour check l'output
    // de la commande ?
    cr_expect_eq(exitcode, 1, "Expected: %d. Got: %d.", 1, exitcode);
}

/*
Test(exec_ast, long_echo)
{
    char *cmd =
        "echo The answer to this is very simple. It was a joke. It had to be a "
        "number, an ordinary, smallish number, and I chose that one. Binary "
        "representations, base thirteen, Tibetan monks are all complete "
        "nonsense. I sat at my desk, stared into the garden and thought ‘42 "
        "will do’. I typed it out. End of story.";
    struct Ast *test_ast = parse_command(cmd);

    cr_redirect_stdout();
    exec_ast(test_ast);
    fflush(NULL);

    char *expected_output =
        "The answer to this is very simple. It was a joke. It had to be a "
        "number, an ordinary, smallish number, and I chose that one. Binary "
        "representations, base thirteen, Tibetan monks are all complete "
        "nonsense. I sat at my desk, stared into the garden and thought ‘42 "
        "will do’. I typed it out. End of story.\n";

    cr_expect_stdout_eq_str(expected_output);

    free_ast(test_ast);
}


Test(exec_ast, echo_combination_options_1)
{
    char *cmd = "echo -nE Hello World! \\n";
    struct Ast *test_ast = parse_command(cmd);

    cr_redirect_stdout();
    exec_ast(test_ast);
    fflush(NULL);

    char *expected_output = "Hello World! n";

    cr_expect_stdout_eq_str(expected_output);

    free_ast(test_ast);
}

Test(exec_ast, echo_combination_options_2)
{
    char *cmd = "echo -n -e Hello World! \\n";
    struct Ast *test_ast = parse_command(cmd);

    cr_redirect_stdout();
    exec_ast(test_ast);
    fflush(NULL);

    char *expected_output = "Hello World! n";

    cr_expect_stdout_eq_str(expected_output);

    free_ast(test_ast);
}

Test(exec_ast, echo_combination_options_3)
{
    char *cmd = "echo -n -E -e Hello World! \\n";
    struct Ast *test_ast = parse_command(cmd);

    cr_redirect_stdout();
    exec_ast(test_ast);
    fflush(NULL);

    char *expected_output = "Hello World! n";

    cr_expect_stdout_eq_str(expected_output);

    free_ast(test_ast);
}

Test(exec_ast, echo_combination_options_4)
{
    char *cmd = "echo -eEn Hello World! \\n";
    struct Ast *test_ast = parse_command(cmd);

    cr_redirect_stdout();
    exec_ast(test_ast);
    fflush(NULL);

    char *expected_output = "Hello World! n";

    cr_expect_stdout_eq_str(expected_output);

    free_ast(test_ast);
}

Test(exec_ast, echo_combination_options_5)
{
    char *cmd = "echo -Ee -n Hello World! \\n";
    struct Ast *test_ast = parse_command(cmd);

    cr_redirect_stdout();
    exec_ast(test_ast);
    fflush(NULL);

    char *expected_output = "Hello World! n";

    cr_expect_stdout_eq_str(expected_output);

    free_ast(test_ast);
}

Test(exec_ast, simple_echo_semicolon_1)
{
    char *cmd = "echo Hello World;";
    struct Ast *test_ast = parse_command(cmd);

    cr_redirect_stdout();
    exec_ast(test_ast);
    fflush(NULL);

    char *expected_output = "Hello World\n";

    cr_expect_stdout_eq_str(expected_output);

    free_ast(test_ast);
}

Test(exec_ast, simple_echo_semicolon_2)
{
    char *cmd = "echo Hello World ;";
    struct Ast *test_ast = parse_command(cmd);

    cr_redirect_stdout();
    exec_ast(test_ast);
    fflush(NULL);

    char *expected_output = "Hello World\n";

    cr_expect_stdout_eq_str(expected_output);

    free_ast(test_ast);
}

Test(exec_ast, double_echo_1)
{
    char *cmd = "echo two /bin/echo inputs;";
    struct Ast *test_ast = parse_command(cmd);

    cr_redirect_stdout();
    exec_ast(test_ast);
    fflush(NULL);

    char *expected_output = "two\ninputs\n";

    cr_expect_stdout_eq_str(expected_output);

    free_ast(test_ast);
}

Test(exec_ast, double_echo_2)
{
    char *cmd = "echo   two      echo   \t   inputs ";
    struct Ast *test_ast = parse_command(cmd);

    cr_redirect_stdout();
    exec_ast(test_ast);
    fflush(NULL);

    char *expected_output = "two\ninputs\n";

    cr_expect_stdout_eq_str(expected_output);

    free_ast(test_ast);
}

Test(exec_ast, empty)
{
    char *cmd = "echo ;";
    struct Ast *test_ast = parse_command(cmd);

    cr_redirect_stdout();
    exec_ast(test_ast);
    fflush(NULL);

    char *expected_output = "\n";

    cr_expect_stdout_eq_str(expected_output);

    free_ast(test_ast);
}

// === command_list ===========================================================

Test(exec_ast, cmd_list_multiple_echo)
{
    char *cmd = "echo I must;echo not; echo fear. Fear is; echo   the
mind-killer."; struct Ast *test_ast = parse_command(cmd);

    cr_redirect_stdout();
    exec_ast(test_ast);
    fflush(NULL);

    char *expected_output = "I must\nnot\nfear. Fear is\nthe mind-killer.\n";

    cr_expect_stdout_eq_str(expected_output);

    free_ast(test_ast);
}

Test(exec_ast, cmd_list_diff_cmds)
{
    char *cmd = "echo Hello World!; ls /bin; echo Test.;";
    struct Ast *test_ast = parse_command(cmd);
    cr_redirect_stdout();
    int exitcode = exec_ast(test_ast);
    fflush(NULL);
    free_ast(test_ast);

    cr_expect_eq(exitcode, 0, "Expected: %d. Got: %d.", 0, exitcode);
}
*/
