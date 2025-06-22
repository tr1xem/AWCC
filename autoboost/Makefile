CFLAGS=-std=c18 -pedantic-errors -Werror=pedantic -O3 -I./include
LIBS=-lusb-1.0
DEPS=include/AWCC.h
SRC=src/AWCC.c src/awcc_autoboost.c

OBJ=${SRC:%.c=%.o}
	PREFIX = /usr/local

%.o:    %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

awcc_autoboost:    $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

clean:
	rm -f $(OBJ) awcc_autoboost

install: awcc_autoboost
	cp -fv awcc_autoboost.sh /usr/local/bin
	mkdir -pv /etc/awcc
	cp -fv awcc_autoboost.conf /etc/awcc/autoboost.conf
	mkdir -pv /etc/systemd/system
	cp -fv awcc_autoboost.service /etc/systemd/system/awcc_autoboost.service

.PHONY:		awcc_autoboost clean install uninstall
