#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../parser/parsing.h"
#include "../exec_ast.h"

TestSuite(exec_ast);

Test(exec_ast, dummy)
{
    cr_assert(1);
}

/*
// === simple_command =========================================================

Test(exec_ast, simple_echo)
{
    char *cmd = "echo Hello World!";
    struct Ast *test_ast = parse_command(cmd);

    cr_redirect_stdout();
    exec_ast(test_ast);
    fflush(NULL);

    char *expected_output = "Hello World!\n";

    cr_expect_stdout_eq_str(expected_output);

    free_ast(test_ast);
}

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

Test(exec_ast, echo_n_option)
{
    char *cmd = "echo -n Hello World! \\n";
    struct Ast *test_ast = parse_command(cmd);

    cr_redirect_stdout();
    exec_ast(test_ast);
    fflush(NULL);

    char *expected_output = "Hello World! n";

    cr_expect_stdout_eq_str(expected_output);

    free_ast(test_ast);
}

Test(exec_ast, echo_e_option)
{
    char *cmd = "echo -e Hello World! \\n";
    struct Ast *test_ast = parse_command(cmd);

    cr_redirect_stdout();
    exec_ast(test_ast);
    fflush(NULL);

    char *expected_output = "Hello World! n\n";

    cr_expect_stdout_eq_str(expected_output);

    free_ast(test_ast);
}

Test(exec_ast, echo_E_option)
{
    char *cmd = "echo -E Hello World! \\n";
    struct Ast *test_ast = parse_command(cmd);

    cr_redirect_stdout();
    exec_ast(test_ast);
    fflush(NULL);

    char *expected_output = "Hello World! n\n";

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

Test(exec_ast, simple_cmd_non_builtin)
{
    char *cmd = "ls /bin;";
    struct Ast *test_ast = parse_command(cmd);
    cr_redirect_stdout();
    int exitcode = exec_ast(test_ast);
    fflush(NULL);
    free_ast(test_ast);

    // Test seulement pour savoir si tout run bien
    // Creer un dossier temporaire pour check l'output
    // de la commande ?
    cr_expect_eq(exitcode, 0, "Expected: %d. Got: %d.", 0, exitcode);
}

// === builtin_true_false =====================================================

Test(exec_ast, builtin_true)
{
    char *cmd = "true;";
    struct Ast *test_ast = parse_command(cmd);
    cr_redirect_stdout();
    int exitcode = exec_ast(test_ast);
    fflush(NULL);
    free_ast(test_ast);

    cr_expect_eq(exitcode, 0, "Expected: %d. Got: %d.", 0, exitcode);
}

Test(exec_ast, builtin_false)
{
    char *cmd = "false;";
    struct Ast *test_ast = parse_command(cmd);
    cr_redirect_stdout();
    int exitcode = exec_ast(test_ast);
    fflush(NULL);
    free_ast(test_ast);

    cr_expect_eq(exitcode, 1, "Expected: %d. Got: %d.", 1, exitcode);
}

// === command_list ===========================================================

Test(exec_ast, cmd_list_multiple_echo)
{
    char *cmd = "echo I must;echo not; echo fear. Fear is; echo   the       mind-killer.";
    struct Ast *test_ast = parse_command(cmd);

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
