check-local: lexer_cov.o unit_tests_lexer.o
	gcc -g -O2 -fsanitize=address -lgcov --coverage -o ut_lexer unit_tests_lexer.o lexer_cov.o -lcriterion
	./ut_lexer
	gcovr --html --html-nested --output=report.html -r ../

lexer_cov.o: CFLAGS = -Wall -Werror -Wextra -Wvla -pedantic -std=c99 -g -fsanitize=address -fPIC --coverage
lexer_cov.o:
	gcc -c -o lexer_cov.o ../lexer.c $(CFLAGS)

unit_tests_lexer.o: CFLAGS = -Wall -Werror -Wextra -Wvla -pedantic -std=c99 -g -fsanitize=address
unit_tests_lexer.o:
	gcc -c -o unit_tests_lexer.o unit_tests_lexer.c $(CFLAGS)

clean:
	rm -rf lexer.o lexer_cov.o unit_tests_lexer.o ut_lexer .deps/ report.* *.gcda *.gcdo
