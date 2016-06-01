MAKE = make
SRCS = ap.c arena.c arith.c array.c assert.c atom.c bit.c btree.c \
    except.c fmt.c list.c mem.c mp.c rbtree.c ring.c seq.c set.c \
    stack.c str.c table.c text.c uarray.c xp.c map.c ntree.c
SRCDIR = ../../src
OBJS = $(SRCS:.c=.o)
INCLUDES=-I../../src
TARGET = libcii.a

.export SRCS SRCDIR OBJS INCLUDES TARGET

release:
	cd build/release && $(MAKE)

all: release debug

debug:
	cd build/debug && $(MAKE)

install: all
	mkdir -p $(PREFIX)/include/cii
	mkdir -p $(PREFIX)/lib
	cp build/release/*.a $(PREFIX)/lib
	cp src/*.h $(PREFIX)/include/cii

clean: clean-release clean-debug
	rm -f a.*

clean-release:
	cd build/release && $(MAKE) clean

clean-debug:
	cd build/debug && $(MAKE) clean
