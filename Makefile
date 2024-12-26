TARGET := awcc
PREFIX ?= $(HOME)/.local
SOURCE := $(wildcard *.c ./include/*.c)
INCLUDE := ./include

all: $(TARGET)

$(TARGET): $(SOURCE)
	gcc -Wall -Wextra -I$(INCLUDE) -o $@ $^ -lusb-1.0

run: $(TARGET)
	./$(TARGET)

install: $(TARGET)
	mkdir --parents "$(PREFIX)/bin"
	cp "$(TARGET)" "$(PREFIX)/bin"

uninstall:
	rm --force "$(PREFIX)/bin/$(TARGET)"
	rmdir --ignore-fail-on-non-empty --parents "$(PREFIX)/bin"

clean:
	rm --force "$(TARGET)"

