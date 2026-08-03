CC      := gcc
CFLAGS  := -std=c2x -march=x86-64 -Isrc -Isrc/include -Isrc/convert/include
LDFLAGS :=

TARGET  := blinker
OBJDIR  := build
SRCS    := $(shell find . -name '*.c')
OBJS    := $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

PREFIX  := /usr/local

.PHONY: all clean install uninstall

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -Isrc/include -c -o $@ $<

install: $(TARGET)
	install -d $(PREFIX)/bin
	install -m 755 $(TARGET) $(PREFIX)/bin/$(TARGET)

uninstall:
	rm -f $(PREFIX)/bin/$(TARGET)

clean:
	rm -rf $(OBJDIR) $(TARGET)