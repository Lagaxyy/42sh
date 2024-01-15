bin_PROGRAMS = 42sh
42sh_SOURCES = 42sh.c

42sh_CFLAGS = -Wall -Werror -Wextra -Wvla -std=c99 -pedantic
42sh_CPPFLAGS = -I$(top_srcdir)/src

42sh_LDADD = $(top_builddir)/src/lexer/liblexer.a \
	$(top_builddir)/src/parser/libparser.a \
	$(top_builddir)/src/function/libfunc.a \
	$(top_builddir)/src/ast/libast.a

SUBDIRS = lexer \
		  parser \
		  function \
		  ast
