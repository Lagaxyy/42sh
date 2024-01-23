check_PROGRAMS = ut_exec_ast

ut_exec_ast_SOURCES = unit_tests_exec_ast.c \
				   $(top_builddir)/src/exec_ast/exec_ast.h \
				   $(top_builddir)/src/parser/parsing.h \
				   $(top_builddir)/src/lexer/lexer.h
ut_exec_ast_CPPFLAGS = -I$(top_srcdir)/src

ut_exec_ast_LDADD = -lcriterion \
				 $(top_builddir)/src/lexer/liblexer.a \
				 $(top_builddir)/src/parser/libparser.a \
				 $(top_builddir)/src/exec_ast/libea.a

ut_exec_ast_LDFLAGS = -fsanitize=address

check-local: CFLAGS += -g -fsanitize=address
check-local: ut_exec_ast
	./ut_exec_ast
