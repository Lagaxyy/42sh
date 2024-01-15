check_PROGRAMS = ut_parser

ut_parser_SOURCES = unit_tests_parser.c \
				   $(top_builddir)/src/parser/parsing.h
				   $(top_builddir)/src/lexer/lexer.h
ut_parser_CPPFLAGS = -I$(top_srcdir)/src

ut_parser_LDADD = -lcriterion \
				 $(top_builddir)/src/lexer/liblexer.a \
				 $(top_builddir)/src/parser/libparser.a

check-local: ut_parser
	./ut_parser
