CC = cc
CCFLAGS = -Wall -pedantic -I../src -O2
CIILIB = ../build/libcii.a
TARGETS = basename calc cref double idents ids

all: strip

strip: $(TARGETS)
	strip $(TARGETS)

basename: basename.c
	$(CC) $(CCFLAGS) -o basename basename.c $(CIILIB)

calc: calc.c
	$(CC) $(CCFLAGS) -o calc calc.c $(CIILIB)

cref: cref.c integer.c
	$(CC) $(CCFLAGS) -o cref cref.c integer.c $(CIILIB)

double: double.c
	$(CC) $(CCFLAGS) -o double double.c $(CIILIB)

idents: idents.c
	$(CC) $(CCFLAGS) -o idents idents.c $(CIILIB)

ids: ids.c
	$(CC) $(CCFLAGS) -o ids ids.c $(CIILIB)

clean:
	rm -f $(TARGETS)
