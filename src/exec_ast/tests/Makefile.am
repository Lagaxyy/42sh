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

OBJ = unit_tests_exec_ast.o \
	  exec_ast_cov.o \
	  run_cmds_cov.o \
	  builtin_echo_cov.o \
	  list_variable_cov.o \
	  merge_token_cov.o \
	  mypipe_cov.o \
	  variable_cov.o \
	  parsing_cov.o \
	  parsing_simple_cov.o \
	  parsing_if_cov.o \
	  parsing_loop_cov.o \
	  parsing_ops_cov.o \
	  free_func_cov.o \
	  print_ast_cov.o \
	  lexer_cov.o

OBJ2 = \
	  exec_ast.o \
	  run_cmds.o \
	  builtin_echo.o \
	  list_variable.o \
	  merge_token.o \
	  ../../mypipe/mypipe.o \
	  ../../variable/variable.o \
	  ../../parser/parsing.o \
	  ../../parser/parsing_simple.o \
	  ../../parser/parsing_if.o \
	  ../../parser/parsing_loop.o \
	  ../../parser/parsing_ops.o \
	  ../../parser/free_func.o \
	  ../../parser/print_ast.o \
	  ../../lexer/lexer.o

cov: $(OBJ)
	gcc -g -O2 -fsanitize=address -lgcov --coverage -o ut_exec_ast $^ -lcriterion
	./ut_exec_ast
	gcovr --html --html-nested --output=report.html -r ../

exec_ast_cov.o: CFLAGS = -Wall -Werror -Wextra -Wvla -pedantic -std=c99 -g -fsanitize=address -fPIC --coverage
exec_ast_cov.o: ../exec_ast.c
	gcc -c -o $@ $< $(CFLAGS)

run_cmds_cov.o: CFLAGS = -Wall -Werror -Wextra -Wvla -pedantic -std=c99 -g -fsanitize=address -fPIC --coverage
run_cmds_cov.o: ../run_cmds.c
	gcc -c -o $@ $< $(CFLAGS)

builtin_echo_cov.o: CFLAGS = -Wall -Werror -Wextra -Wvla -pedantic -std=c99 -g -fsanitize=address -fPIC --coverage
builtin_echo_cov.o: ../builtin_echo.c
	gcc -c -o $@ $< $(CFLAGS)

list_variable_cov.o: CFLAGS = -Wall -Werror -Wextra -Wvla -pedantic -std=c99 -g -fsanitize=address -fPIC --coverage
list_variable_cov.o: ../list_variable.c
	gcc -c -o $@ $< $(CFLAGS)

merge_token_cov.o: CFLAGS = -Wall -Werror -Wextra -Wvla -pedantic -std=c99 -g -fsanitize=address -fPIC --coverage
merge_token_cov.o: ../merge_token.c
	gcc -c -o $@ $< $(CFLAGS)

mypipe_cov.o: CFLAGS = -Wall -Werror -Wextra -Wvla -pedantic -std=c99 -g -fsanitize=address -fPIC --coverage
mypipe_cov.o: ../../mypipe/mypipe.c
	gcc -c -o $@ $< $(CFLAGS)

variable_cov.o: CFLAGS = -Wall -Werror -Wextra -Wvla -pedantic -std=c99 -g -fsanitize=address -fPIC --coverage
variable_cov.o: ../../variable/variable.c
	gcc -c -o $@ $< $(CFLAGS)

parsing_cov.o: CFLAGS = -Wall -Werror -Wextra -Wvla -pedantic -std=c99 -g -fsanitize=address -fPIC --coverage
parsing_cov.o: ../../parser/parsing.c
	gcc -c -o $@ $< $(CFLAGS)

parsing_simple_cov.o: CFLAGS = -Wall -Werror -Wextra -Wvla -pedantic -std=c99 -g -fsanitize=address -fPIC --coverage
parsing_simple_cov.o: ../../parser/parsing_simple.c
	gcc -c -o $@ $< $(CFLAGS)

parsing_if_cov.o: CFLAGS = -Wall -Werror -Wextra -Wvla -pedantic -std=c99 -g -fsanitize=address -fPIC --coverage
parsing_if_cov.o: ../../parser/parsing_if.c
	gcc -c -o $@ $< $(CFLAGS)

parsing_loop_cov.o: CFLAGS = -Wall -Werror -Wextra -Wvla -pedantic -std=c99 -g -fsanitize=address -fPIC --coverage
parsing_loop_cov.o: ../../parser/parsing_loop.c
	gcc -c -o $@ $< $(CFLAGS)

parsing_ops_cov.o: CFLAGS = -Wall -Werror -Wextra -Wvla -pedantic -std=c99 -g -fsanitize=address -fPIC --coverage
parsing_ops_cov.o: ../../parser/parsing_ops.c
	gcc -c -o $@ $< $(CFLAGS)

free_func_cov.o: CFLAGS = -Wall -Werror -Wextra -Wvla -pedantic -std=c99 -g -fsanitize=address -fPIC --coverage
free_func_cov.o: ../../parser/free_func.c
	gcc -c -o $@ $< $(CFLAGS)

print_ast_cov.o: CFLAGS = -Wall -Werror -Wextra -Wvla -pedantic -std=c99 -g -fsanitize=address -fPIC --coverage
print_ast_cov.o: ../../parser/print_ast.c
	gcc -c -o $@ $< $(CFLAGS)

lexer_cov.o: CFLAGS = -Wall -Werror -Wextra -Wvla -pedantic -std=c99 -g -fsanitize=address -fPIC --coverage
lexer_cov.o: ../../lexer/lexer.c
	gcc -c -o $@ $< $(CFLAGS)

unit_tests_exec_ast.o: CFLAGS = -Wall -Werror -Wextra -Wvla -pedantic -std=c99 -g -fsanitize=address
unit_tests_exec_ast.o: unit_tests_exec_ast.c
	gcc -c -o $@ $< $(CFLAGS)

clean:
	rm -rf ut_exec_ast $(OBJ) $(OBJ2) report.* *.gcda *.gcno

