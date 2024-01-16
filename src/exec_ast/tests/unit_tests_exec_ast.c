#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "../exec_ast.h"
#include "../../parser/parsing.h"

#include <stdlib.h>
#include <string.h>

TestSuite(exec_ast);

Test(exec_ast, dummy)
{
    cr_assert(1);
}

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
    char *cmd = "echo The answer to this is very simple. It was a joke. It had to be a number, an ordinary, smallish number, and I chose that one. Binary representations, base thirteen, Tibetan monks are all complete nonsense. I sat at my desk, stared into the garden and thought ‘42 will do’. I typed it out. End of story.";
    struct Ast *test_ast = parse_command(cmd);

    cr_redirect_stdout();
    exec_ast(test_ast);
    fflush(NULL);

    char *expected_output = "The answer to this is very simple. It was a joke. It had to be a number, an ordinary, smallish number, and I chose that one. Binary representations, base thirteen, Tibetan monks are all complete nonsense. I sat at my desk, stared into the garden and thought ‘42 will do’. I typed it out. End of story.\n";

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

    char *expected_output = "Hello World! \\n";

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

    char *expected_output = "Hello World! \n\n";

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

    char *expected_output = "Hello World! \\n\n";

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

    char *expected_output = "Hello World! \\n";

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

    char *expected_output = "Hello World! \n";

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

    char *expected_output = "Hello World! \n";

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

    char *expected_output = "Hello World! \\n";

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

    char *expected_output = "Hello World! \n";

    cr_expect_stdout_eq_str(expected_output);

    free_ast(test_ast);
}
