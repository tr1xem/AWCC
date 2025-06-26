PREFIX = /usr/local

CFLAGS=-std=c18 -pedantic-errors -Werror=pedantic -O0 -g -DDEBUG -I./include -DENABLE_LOGS
LIBS=-lusb-1.0 -lpcre2-32

DEPS := $(wildcard include/*.h) $(wildcard src/*.h)
SRC := $(wildcard src/*.c)

OBJ=${SRC:%.c=%.o}

awcc_autoboost: $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f $(OBJ) awcc_autoboost

compile_commands.json: Makefile
	@command -v compiledb >/dev/null || { echo "compiledb not found! Install it with 'pipx install compiledb'"; exit 1; }
	@compiledb -n make

compiledb: compile_commands.json

run: awcc_autoboost compiledb # update compile_commands.json for neovim
	./awcc_autoboost

install: awcc_autoboost
	cp -fv awcc_autoboost.sh /usr/local/bin
	mkdir -pv /etc/awcc
	cp -fv awcc_autoboost.conf /etc/awcc/autoboost.conf
	mkdir -pv /etc/systemd/system
	cp -fv awcc_autoboost.service /etc/systemd/system/awcc_autoboost.service

.PHONY: awcc_autoboost clean install uninstall compiledb
