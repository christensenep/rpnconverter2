CC=gcc
CFLAGS=-Wall -O0 -g -std=c99 -Wpedantic
LIBS=-lcheck -lm -lpthread

UNAME := $(shell uname)

ifneq ($(UNAME),Darwin)
  LIBS += -lrt
endif

SRCDIR=src
TESTDIR=tests

all: librpn.a

librpn.a: $(SRCDIR)/rpn.o $(SRCDIR)/dynamic_string.o $(SRCDIR)/string_stack.o
	ar rcs $@ $^

$(SRCDIR)/rpn.o: $(SRCDIR)/rpn.c $(SRCDIR)/rpn.h $(SRCDIR)/dynamic_string.h $(SRCDIR)/string_stack.h
	gcc $(CFLAGS) -c $(SRCDIR)/rpn.c -o $@

$(SRCDIR)/dynamic_string.o: $(SRCDIR)/dynamic_string.c $(SRCDIR)/dynamic_string.h
	gcc $(CFLAGS) -c $(SRCDIR)/dynamic_string.c -o $@

$(SRCDIR)/string_stack.o: $(SRCDIR)/string_stack.c $(SRCDIR)/string_stack.h
	gcc $(CFLAGS) -c $(SRCDIR)/string_stack.c -o $@

$(TESTDIR)/check_rpn.o: $(TESTDIR)/check_rpn.c $(SRCDIR)/rpn.h
	gcc $(CFLAGS) -c $(TESTDIR)/check_rpn.c -o $@

test: rpn-test
	./rpn-test

rpn-test: librpn.a $(TESTDIR)/check_rpn.o
	gcc -o rpn-test $(TESTDIR)/check_rpn.o librpn.a $(LIBS)

clean:
	$(RM) $(SRCDIR)/*.o $(TESTDIR)/*.o rpn-test librpn.a
