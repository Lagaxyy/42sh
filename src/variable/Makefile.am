lib_LIBRARIES = libvar.a

libvar_a_SOURCES = variable.c \
				   variable.h

libvar_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic
libvar_a_CPPFLAGS = -I$(top_srcdir)

noinst_LIBRARIES = libvar.a

DC = Makefile \
	 .dirstamp \
	 .deps

distclean:
	rm -rf $(DC)
