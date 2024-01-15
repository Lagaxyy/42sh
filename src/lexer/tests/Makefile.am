check_PROGRAMS = ut_lexer

ut_lexer_SOURCES = unit_tests_lexer.c \
				   $(top_builddir)/src/lexer/lexer.h
ut_lexer_CPPFLAGS = -I$(top_srcdir)/src

ut_lexer_LDADD = -lcriterion \
				 $(top_builddir)/src/lexer/liblexer.a

check-local: ut_lexer
	./ut_lexer
