lib_LIBRARIES = libparser.a

libparser_a_SOURCES = parsing.c \
  parsing.h \
  parsing_simple.c \
  parsing_if.c \
  parsing_loop.c \
  parsing_ops.c \
  free_func.c \
  print_ast.c \
  ../lexer/lexer.c \
  ../lexer/lexer.h
libparser_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic
libparser_a_CPPFLAGS = -I$(top_srcdir)

noinst_LIBRARIES = libparser.a

DC = tests/Makefile \
	 Makefile \
	 .deps

check-local: libparser.a
	$(MAKE) -C tests $@

clean-local:
	$(MAKE) -C tests clean
	rm -rf tests/ut_parser-unit_tests_parser.o

distclean:
	rm -rf $(DC)

cov:
	$(MAKE) -C tests $@
