lib_LIBRARIES = libea.a

libea_a_SOURCES = exec_ast.c \
				   builtin_echo.c \
				   run_cmds.c \
				   list_variable.c \
				   merge_token.c \
				   merge_token.h \
				   exec_ast.h

libea_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic
libea_a_CPPFLAGS = -I$(top_srcdir)

noinst_LIBRARIES = libea.a

DC = tests/Makefile \
	 Makefile \
	 ../lexer/Makefile \
	 ../lexer/tests/Makefile \
	 .deps \
	 tests/.deps \
	 ../lexer/.deps \
	 ../lexer/tests/.deps

check-local: libea.a
	$(MAKE) -C tests $@

clean-local:
	$(MAKE) -C tests clean
	rm -rf tests/ut_exec_ast-unit_tests_exec_ast.o

distclean:
	rm -rf $(DC)

cov:
	$(MAKE) -C tests $@
