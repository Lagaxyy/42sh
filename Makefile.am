export CC = gcc
export CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla

BINARY=42sh

.PHONY: all check clean

all: $(BINARY)

$(BINARY):
	$(MAKE) -C src
	mv src/$(BINARY) .

check: $(BINARY)
	#commande check

clean:
	$(MAKE) -C src clean
	$(RM) $(BINARY)

.PHONY: all check clean
