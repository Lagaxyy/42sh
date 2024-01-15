CC = gcc
CFLAGS = -Wall -Werror -Wextra -Wvla -pedantic -std=c99

BIN = main

OBJ = $(BIN).o run_cmds.o exec_ast.o ../parser/parsing.o ../parser/free_func.o ../lexer/lexer.o

all: $(BIN)

check: CFLAGS += -g -fsanitize=address
check: LDFLAGS = -fsanitize=address
check: $(OBJ)
	$(CC) -o $(BIN) $(OBJ) $(LDFLAGS)

$(BIN): $(OBJ)

clean:
		$(RM) $(BIN) $(OBJ)

.PHONY: all check clean
