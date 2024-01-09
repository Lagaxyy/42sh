CC = gcc
CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla

SRC = # your code to include

OBJ = ${SRC:.c=.o}

all: 42sh

42sh: $(OBJ)
	$(CC) -o 42sh $(OBJ)

clean:
	$(RM) $(OBJ)
	$(RM) 42sh

.PHONY: all clean
