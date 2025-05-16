TARGET := awcc
PREFIX ?= $(HOME)/.local
SOURCE := $(wildcard *.c ./include/*.c)
INCLUDE := ./include

# Compiler and linker flags
CFLAGS := -Wall -Wextra -I$(INCLUDE)
LDFLAGS := -lusb-1.0

all: $(TARGET)

$(TARGET): $(SOURCE)
	gcc $(CFLAGS) -o $@ $^ $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

install: $(TARGET)
	mkdir --parents "$(PREFIX)/bin"
	cp "$(TARGET)" "$(PREFIX)/bin"

uninstall:
	rm --force "$(PREFIX)/bin/$(TARGET)"

clean:
	rm --force "$(TARGET)"
