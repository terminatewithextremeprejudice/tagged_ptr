.PHONY: test

OUTEXT=
CFLAGS=-Og -std=c99 $(MYCFLAGS)
EXTRA_CFLAGS=-Wall -Wextra -I..
RELEASE_CFLAGS=-O2 -flto -march=native

all: default_tag_types

test: default_tag_types
	$(RUNNER) ./default_tag_types$(OUTEXT)

test-all: default_tag_types_test
	$(RUNNER) ./default_tag_types$(OUTEXT)

default_tag_types_test: default_tag_types.c ../tagged_ptr.h Makefile
	$(CC) default_tag_types.c -o default_tag_test $(EXTRA_CFLAGS) $(CFLAGS)

clean:
	rm -f default_tag_types$(OUTEXT)


