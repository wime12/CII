CCFLAGS = -Wall -pedantic -O2
TARGET = libcii.a

$(TARGET): $(OBJS)
	ar -cr $(.TARGET) $(.ALLSRC)
	# strip --strip-unneeded $(.TARGET)

$(OBJS) : $(SRCDIR)/$(.PREFIX).c
	$(CC) $(CCFLAGS) $(INCLUDES) -o $(.TARGET) -c $(.ALLSRC)

clean:
	rm -f $(TARGET) $(OBJS)
