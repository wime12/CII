CCFLAGS = -Wall -pedantic -g -O0
SRCS = ap.c arena.c arith.c array.c assert.c atom.c bit.c btree.c \
    except.c fmt.c list.c mem.c mp.c rbtree.c ring.c seq.c set.c \
    stack.c str.c table.c text.c uarray.c xp.c
SRCDIR = ../../src
OBJS = $(SRCS:.c=.o)
INCLUDES=-I../../src
TARGET = libcii.a

$(TARGET): $(OBJS)
	ar -cr $(.TARGET) $(.ALLSRC)

$(OBJS) : $(SRCDIR)/$(.PREFIX).c
	$(CC) $(CCFLAGS) $(INCLUDES) -o $(.TARGET) -c $(.ALLSRC)

clean:
	rm -f $(TARGET) $(OBJS)
