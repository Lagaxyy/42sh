lib_LIBRARIES = libea.a

libea_a_SOURCES = exec_ast.c \
				   builtin_echo.c \
				   run_cmds.c \
				   exec_ast.h

libea_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic
libea_a_CPPFLAGS = -I$(top_srcdir)

noinst_LIBRARIES = libea.a

SUBDIRS = tests
