TARGET := awcc
PREFIX ?= $(HOME)/.local
SOURCE := $(wildcard *.c ./include/*.c)
INCLUDE := ./include
PKG_CONFIG := $(shell which pkg-config)

CFLAGS := -Wall -Wextra -I$(INCLUDE)
LDFLAGS := -lusb-1.0
ifeq ($(PKG_CONFIG),)
    $(error "pkg-config not found. Please install it to use libnotify and its dependencies.")
else
    LIBNOTIFY_CFLAGS := $(shell pkg-config --cflags libnotify)
    LIBNOTIFY_LDFLAGS := $(shell pkg-config --libs libnotify)
endif

all: $(TARGET)

$(TARGET): $(SOURCE)
	gcc $(CFLAGS) $(LIBNOTIFY_CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBNOTIFY_LDFLAGS)

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

