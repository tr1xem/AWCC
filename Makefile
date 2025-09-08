TARGET := awcc
PREFIX ?= $(HOME)/.local
SOURCE := $(wildcard *.c ./include/*.c)
INCLUDE := ./include

# Compiler and linker flags
CFLAGS := -std=c18 -pedantic-errors -Werror=pedantic -Wall -Wextra -I$(INCLUDE)
LDFLAGS := -lusb-1.0 -lpcre2-32

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
