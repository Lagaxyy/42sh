CC = gcc
CFLAGS = -Wall -Werror -Wextra -Wvla -std=c99 -pedantic -g -fsanitize=address

SRC_PATH = src/

SRC =	$(SRC_PATH)/42sh.c \
		$(SRC_PATH)/lexer/lexer.c \
    	$(SRC_PATH)/parser/parsing.c \
    	$(SRC_PATH)/parser/free_func.c \
    	$(SRC_PATH)/parser/print_ast.c \
    	$(SRC_PATH)/exec_ast/exec_ast.c \
    	$(SRC_PATH)/exec_ast/builtin_echo.c \
    	$(SRC_PATH)/exec_ast/run_cmds.c \
    	$(SRC_PATH)/exec_ast/list_variable.c \
		$(SRC_PATH)/mypipe/mypipe.c \
		$(SRC_PATH)/variable/variable.c \

OBJ = ${SRC:.c=.o}

all: 42sh

42sh: $(OBJ)
	$(CC) -o 42sh $(OBJ) -lasan

clean:
	$(RM) $(OBJ)
	$(RM) 42sh
