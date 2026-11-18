CC = $(CROSS_COMPILE)gcc
CFLAGS ?= -Wall -Wextra -g

.PHONY: all clean

all: writer

writer: writer.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

writer.o: writer.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) writer *.o
