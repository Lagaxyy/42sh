check_PROGRAMS = ut_parser

ut_parser_SOURCES = unit_tests_parser.c \
				   $(top_builddir)/src/parser/parsing.h \
				   $(top_builddir)/src/lexer/lexer.h
ut_parser_CPPFLAGS = -I$(top_srcdir)/src

ut_parser_LDADD = -lcriterion \
				 $(top_builddir)/src/lexer/liblexer.a \
				 $(top_builddir)/src/parser/libparser.a

ut_parser_LDFLAGS = -fsanitize=address

check-local: CFLAGS += -g -fsanitize=address
check-local: ut_parser
	./ut_parser

OBJ = unit_tests_parser.o \
	  parsing_cov.o \
	  parsing_simple_cov.o \
	  parsing_if_cov.o \
	  parsing_loop_cov.o \
	  parsing_ops_cov.o \
	  free_func_cov.o \
	  print_ast_cov.o \
	  lexer_cov.o

OBJ2 = \
	  parsing.o \
	  parsing_simple.o \
	  parsing_if.o \
	  parsing_loop.o \
	  parsing_ops.o \
	  free_func.o \
	  print_ast.o \
	  ../../lexer/lexer.o

cov: $(OBJ)
	gcc -g -O2 -fsanitize=address -lgcov --coverage -o ut_parser $^ -lcriterion
	./ut_parser
	gcovr --html --html-nested --output=report.html -r ../

parsing_cov.o: CFLAGS = -Wall -Werror -Wextra -Wvla -pedantic -std=c99 -g -fsanitize=address -fPIC --coverage
parsing_cov.o: ../parsing.c
	gcc -c -o $@ $< $(CFLAGS)

parsing_simple_cov.o: CFLAGS = -Wall -Werror -Wextra -Wvla -pedantic -std=c99 -g -fsanitize=address -fPIC --coverage
parsing_simple_cov.o: ../parsing_simple.c
	gcc -c -o $@ $< $(CFLAGS)

parsing_if_cov.o: CFLAGS = -Wall -Werror -Wextra -Wvla -pedantic -std=c99 -g -fsanitize=address -fPIC --coverage
parsing_if_cov.o: ../parsing_if.c
	gcc -c -o $@ $< $(CFLAGS)

parsing_loop_cov.o: CFLAGS = -Wall -Werror -Wextra -Wvla -pedantic -std=c99 -g -fsanitize=address -fPIC --coverage
parsing_loop_cov.o: ../parsing_loop.c
	gcc -c -o $@ $< $(CFLAGS)

parsing_ops_cov.o: CFLAGS = -Wall -Werror -Wextra -Wvla -pedantic -std=c99 -g -fsanitize=address -fPIC --coverage
parsing_ops_cov.o: ../parsing_ops.c
	gcc -c -o $@ $< $(CFLAGS)

free_func_cov.o: CFLAGS = -Wall -Werror -Wextra -Wvla -pedantic -std=c99 -g -fsanitize=address -fPIC --coverage
free_func_cov.o: ../free_func.c
	gcc -c -o $@ $< $(CFLAGS)

print_ast_cov.o: CFLAGS = -Wall -Werror -Wextra -Wvla -pedantic -std=c99 -g -fsanitize=address -fPIC --coverage
print_ast_cov.o: ../print_ast.c
	gcc -c -o $@ $< $(CFLAGS)

lexer_cov.o: CFLAGS = -Wall -Werror -Wextra -Wvla -pedantic -std=c99 -g -fsanitize=address -fPIC --coverage
lexer_cov.o: ../../lexer/lexer.c
	gcc -c -o $@ $< $(CFLAGS)

unit_tests_parser.o: CFLAGS = -Wall -Werror -Wextra -Wvla -pedantic -std=c99 -g -fsanitize=address
unit_tests_parser.o: unit_tests_parser.c
	gcc -c -o $@ $< $(CFLAGS)

clean:
	rm -rf ut_parser $(OBJ) $(OBJ2) report.* *.gcda *.gcno

