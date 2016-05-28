CCFLAGS = -Wall -pedantic -g -O0
$(TARGET): $(OBJS)
	ar -cr $(.TARGET) $(.ALLSRC)

$(OBJS) : $(SRCDIR)/$(.PREFIX).c
	$(CC) $(CCFLAGS) $(INCLUDES) -o $(.TARGET) -c $(.ALLSRC)

clean:
	rm -f $(TARGET) $(OBJS)
