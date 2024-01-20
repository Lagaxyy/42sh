bin_PROGRAMS = 42sh
42sh_SOURCES = 42sh.c

42sh_CFLAGS = -Wall -Werror -Wextra -Wvla -std=c99 -pedantic #-g -fsanitize=address
42sh_CPPFLAGS = -I$(top_srcdir)/src

42sh_LDADD = $(top_builddir)/src/lexer/liblexer.a \
	$(top_builddir)/src/parser/libparser.a \
	$(top_builddir)/src/exec_ast/libea.a \
	$(top_builddir)/src/print_ast/libprint.a

# 42sh_LDFLAGS = -fsanitize=address

SUBDIRS = lexer \
		  parser \
		  exec_ast \
		  print_ast
