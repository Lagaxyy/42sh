CC = gcc
CFLAGS = -Wall -Werror -Wextra -Wvla -pedantic -std=c99 -g -fsanitize=address
LDFLAGS = -fsanitize=address

BIN = main

OBJ = $(BIN).o parsing.o parsing_list.o free_func.o ../lexer/lexer.o parsing_simple.o pretty_print.o

all: $(BIN)

$(BIN): $(OBJ)

clean:
		$(RM) $(BIN) $(OBJ)

.PHONY: all check clean
