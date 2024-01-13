CC = gcc
CFLAGS = -g -std=c99 -pedantic -Werror -Wall -Wextra -Wvla

prog : lexer/lexer.o parser/parsing.o
	gcc -o main lexer/lexer.o parser/parsing.o

lexer/lexer.o : lexer/lexer.c
parser/parsing.o : parser/parsing.c