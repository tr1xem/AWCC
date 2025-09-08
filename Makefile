TARGET := awcc
DAEMON_TARGET := awccd
PREFIX ?= $(HOME)/.local
SOURCE := main.c $(wildcard ./src/*.c)
DAEMON_SOURCE := awccd.c $(wildcard ./src/*.c)
INCLUDE := ./include

# Compiler and linker flags
CFLAGS := -std=c18 -pedantic-errors -Werror=pedantic -Wall -Wextra -I$(INCLUDE)
LDFLAGS := -lusb-1.0 -lpcre2-32

all: $(TARGET) $(DAEMON_TARGET)

$(TARGET): $(SOURCE)
	gcc $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(DAEMON_TARGET): $(DAEMON_SOURCE)
	gcc $(CFLAGS) -o $@ $^ $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

run-daemon: $(DAEMON_TARGET)
	sudo ./$(DAEMON_TARGET)

install: $(TARGET) $(DAEMON_TARGET)
	mkdir --parents "$(PREFIX)/bin"
	cp "$(TARGET)" "$(PREFIX)/bin"
	cp "$(DAEMON_TARGET)" "$(PREFIX)/bin"

uninstall:
	rm --force "$(PREFIX)/bin/$(TARGET)"
	rm --force "$(PREFIX)/bin/$(DAEMON_TARGET)"

clean:
	rm --force "$(TARGET)" "$(DAEMON_TARGET)"
