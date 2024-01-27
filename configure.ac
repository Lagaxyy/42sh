AC_INIT([42sh], [1.0], [arthur.lao@epita.fr])
AM_INIT_AUTOMAKE([subdir-objects] [foreign])

AC_PROG_CC
AX_COMPILER_FLAGS([], [], [], [-Wall -Werror -Wextra -Wvla -std=c99 -pedantic])

AM_PROG_AR
AC_PROG_RANLIB

AC_CONFIG_FILES([
    Makefile
    src/Makefile
    src/lexer/Makefile
    src/lexer/tests/Makefile
    src/parser/Makefile
    src/parser/tests/Makefile
    src/exec_ast/Makefile
    src/exec_ast/tests/Makefile
    src/mypipe/Makefile
    src/variable/Makefile
    ])

AC_OUTPUT
