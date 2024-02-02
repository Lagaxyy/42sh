lib_LIBRARIES = libvar.a

libvar_a_SOURCES = variable.c \
				   variable.h

libvar_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic
libvar_a_CPPFLAGS = -I$(top_srcdir)

noinst_LIBRARIES = libvar.a

#check-local: libvar.a
#	$(MAKE) -C tests $@

#clean-local:
#	rm -rf tests/ut_var tests/ut_var-unit_tests_var.o tests/Makefile tests/.deps

#cov:
#	$(MAKE) -C tests $@
