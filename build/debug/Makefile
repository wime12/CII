CFLAGS += -Wall -pedantic -g -O0
$(TARGET): $(OBJS)
	ar -cr $(.TARGET) $(.ALLSRC)

$(OBJS) : $(SRCDIR)/$(.PREFIX).c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(.TARGET) -c $(.ALLSRC)

clean:
	rm -f $(TARGET) $(OBJS)
