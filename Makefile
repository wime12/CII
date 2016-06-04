MAKE = make
CFLAGS = -std=c99
SRCS = ap.c arena.c arith.c array.c assert.c atom.c bit.c btree.c \
    except.c fmt.c list.c mem.c mp.c rbtree.c ring.c seq.c set.c \
    stack.c str.c table.c text.c uarray.c xp.c map.c ntree.c
SRCDIR = ../../src
OBJS = $(SRCS:.c=.o)
INCLUDES=-I../../src
TARGET = libcii.a

.export SRCS SRCDIR OBJS INCLUDES TARGET

release:
	cd build/release && $(MAKE) CFLAGS=$(CFLAGS)

all: release debug

debug:
	cd build/debug && $(MAKE) CFLAGS=$(CFLAGS)

install: all
	mkdir -p $(PREFIX)/include/cii
	mkdir -p $(PREFIX)/lib
	cp build/release/*.a $(PREFIX)/lib
	cp src/*.h $(PREFIX)/include/cii

clean: clean-release clean-debug
	rm -f a.* ntree ntree-dbg rbtree rbtree-dbg

clean-release:
	cd build/release && $(MAKE) clean

clean-debug:
	cd build/debug && $(MAKE) clean

ntree: release
	cc -std=c99 -Wall -pedantic -I src -O2 -o ntree examples/ntree.c build/release/libcii.a

ntree-dbg: debug
	cc -std=c99 -Wall -pedantic -I src -g -o ntree-dbg examples/ntree.c build/debug/libcii.a

rbtree: release
	cc -std=c99 -Wall -pedantic -I src -O2 -o rbtree examples/rbtree.c build/release/libcii.a

rbtree-dbg: debug
	cc -std=c99 -Wall -pedantic -I src -g -o rbtree-dbg examples/rbtree.c build/debug/libcii.a
