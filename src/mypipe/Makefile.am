lib_LIBRARIES = libmypipe.a

libmypipe_a_SOURCES = mypipe.c \
  mypipe.h \
  ../exec_ast/exec_ast.h
libmypipe_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic
libmypipe_a_CPPFLAGS = -I$(top_srcdir)

noinst_LIBRARIES = libmypipe.a

#check-local: libmypipe.a
#	$(MAKE) -C tests $@

#clean-local:
#	rm -rf tests/ut_mypipe tests/ut_mypipe-unit_tests_mypipe.o tests/Makefile tests/.deps

#cov:
#	$(MAKE) -C tests $@
