check_PROGRAMS = test_42sh

test_42sh_SOURCES = $(top_builddir)/src/42sh.c \
				   $(top_builddir)/src/exec_ast/exec_ast.h \
				   $(top_builddir)/src/parser/parsing.h \
				   $(top_builddir)/src/mypipe/mypipe.h \
				   $(top_builddir)/src/variable/variable.h \
				   $(top_builddir)/src/lexer/lexer.h
test_42sh_CPPFLAGS = -I$(top_srcdir)/src

test_42sh_LDADD = \
				 $(top_builddir)/src/lexer/liblexer.a \
				 $(top_builddir)/src/parser/libparser.a \
				 $(top_builddir)/src/mypipe/libmypipe.a \
				 $(top_builddir)/src/variable/libvar.a \
				 $(top_builddir)/src/exec_ast/libea.a

test_42sh_LDFLAGS = -fsanitize=address

check-local: CFLAGS += -g -fsanitize=address
check-local: test_42sh
	./testsuite_check.sh

clean-local:
	rm -rf test_42sh

distclean:
	rm -rf Makefile
