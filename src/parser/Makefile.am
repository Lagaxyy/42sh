lib_LIBRARIES = libparser.a

libparser_a_SOURCES = parsing.c \
  parsing.h \
  free_func.c \
  print_ast.c \
  ../lexer/lexer.c \
  ../lexer/lexer.h
libparser_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic
libparser_a_CPPFLAGS = -I$(top_srcdir)

noinst_LIBRARIES = libparser.a

#SUBDIRS = tests
