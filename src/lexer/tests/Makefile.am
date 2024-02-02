check_PROGRAMS = ut_lexer

ut_lexer_SOURCES = unit_tests_lexer.c \
				   $(top_builddir)/src/lexer/lexer.h
ut_lexer_CPPFLAGS = -I$(top_srcdir)/src

ut_lexer_LDADD = -lcriterion \
				 $(top_builddir)/src/lexer/liblexer.a

ut_lexer_LDFLAGS = -fsanitize=address

check-local: CFLAGS += -g -fsanitize=address
check-local: ut_lexer
	./ut_lexer

cov: unit_tests_lexer.o lexer_cov.o
	gcc -g -O2 -fsanitize=address -lgcov --coverage -o ut_lexer $^ -lcriterion
	./ut_lexer
	gcovr --html --html-nested --output=report.html -r ../

lexer_cov.o: CFLAGS = -Wall -Werror -Wextra -Wvla -pedantic -std=c99 -g -fsanitize=address -fPIC --coverage
lexer_cov.o: ../lexer.c
	gcc -c -o $@ $< $(CFLAGS)

unit_tests_lexer.o: CFLAGS = -Wall -Werror -Wextra -Wvla -pedantic -std=c99 -g -fsanitize=address
unit_tests_lexer.o: unit_tests_lexer.c
	gcc -c -o $@ $< $(CFLAGS)

clean:
	rm -rf lexer.o lexer_cov.o unit_tests_lexer.o ut_lexer .deps/ report.* *.gcda *.gcno

