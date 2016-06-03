CFLAGS += -Wall -pedantic -O2

$(TARGET): $(OBJS)
	ar -cr $(.TARGET) $(.ALLSRC)
	# strip --strip-unneeded $(.TARGET)

$(OBJS) : $(SRCDIR)/$(.PREFIX).c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(.TARGET) -c $(.ALLSRC)

clean:
	rm -f $(TARGET) $(OBJS)
