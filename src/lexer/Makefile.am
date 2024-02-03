lib_LIBRARIES = liblexer.a

liblexer_a_SOURCES = lexer.c \
  lexer.h
liblexer_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic
liblexer_a_CPPFLAGS = -I$(top_srcdir)

noinst_LIBRARIES = liblexer.a

check-local: liblexer.a
	$(MAKE) -C tests $@

clean-local:
	$(MAKE) -C tests clean
	rm -rf tests/ut_lexer tests/ut_lexer-unit_tests_lexer.o tests/.deps

distclean:
	rm -rf tests/Makefile Makefile .deps

cov:
	$(MAKE) -C tests $@
