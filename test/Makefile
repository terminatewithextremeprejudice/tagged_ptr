.PHONY: test

OUTEXT=
CFLAGS=-Og -std=c99 $(MYCFLAGS)
#EXTRA_CFLAGS=-Wall -Wextra -I..
EXTRA_CFLAGS=-I..
RELEASE_CFLAGS=-O2 -flto -march=native

all: default_tag_types_test custom_tag_types_test

test: default_tag_types_test custom_tag_types_test
	$(RUNNER) ./default_tag_types$(OUTEXT)
	$(RUNNER) ./custom_tag_types$(OUTEXT)

test-all: test 

default_tag_types_test: default_tag_types.c ../tagged_ptr.h Makefile
	$(CC) default_tag_types.c -o default_tag_types $(EXTRA_CFLAGS) $(CFLAGS)

custom_tag_types_test: custom_tag_types.c ../tagged_ptr.h Makefile
	$(CC) custom_tag_types.c -o custom_tag_types $(EXTRA_CFLAGS) $(CFLAGS)

clean:
	rm -f default_tag_types$(OUTEXT) custom_tag_types$(OUTEXT)

