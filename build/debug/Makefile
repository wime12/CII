CFLAGS += -Wall -pedantic -O0 -g
$(TARGET): $(OBJS)
	ar -cr $(.TARGET) $(.ALLSRC)

$(OBJS) : $(SRCDIR)/$(.PREFIX).c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(.TARGET) -c $(.ALLSRC)

clean:
	rm -f $(TARGET) $(OBJS)
