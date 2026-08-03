CC      := gcc
CFLAGS  := -std=c2x -march=x86-64 
LDFLAGS :=

TARGET  := blinker
SRCS    := $(shell find . -name '*.c')
OBJS    := $(SRCS:.c=.o)

PREFIX  := /usr/local

.PHONY: all clean install uninstall

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -Isrc/include -c -o $@ $<

install: $(TARGET)
	install -d $(PREFIX)/bin
	install -m 755 $(TARGET) $(PREFIX)/bin/$(TARGET)

uninstall:
	rm -f $(PREFIX)/bin/$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)