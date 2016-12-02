CC=gcc
CFLAGS=-Wall -O0 -g -std=c99 -Wpedantic
LIBS=-lcheck

SRCDIR=src
TESTDIR=tests

all: librpn.all

librpn.a: $(SRCDIR)/rpn.o
	ar rcs $@ $^

$(SRCDIR)/rpn.o: $(SRCDIR)/rpn.c $(SRCDIR)/rpn.h
	gcc $(CFLAGS) -c $(SRCDIR)/rpn.c -o $@

$(TESTDIR)/check_rpn.o: $(TESTDIR)/check_rpn.c $(SRCDIR)/rpn.h
	gcc $(CFLAGS) -c $(TESTDIR)/check_rpn.c -o $@

test: rpn-test
	./rpn-test

rpn-test: librpn.a $(TESTDIR)/check_rpn.o
	gcc -o rpn-test $(TESTDIR)/check_rpn.o $(SRCDIR)/rpn.o $(LIBS)

clean:
	$(RM) $(SRCDIR)/*.o $(TESTDIR)/*.o rpn-test librpn.a