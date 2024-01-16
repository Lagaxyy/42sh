lib_LIBRARIES = libea.a

libast_a_SOURCES = exec_ast.c \
				   builtin_echo.c \
				   exec_ast.h

libast_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic
libast_a_CPPFLAGS = -I$(top_srcdir)

noinst_LIBRARIES = libea.a

SUBDIRS = tests
