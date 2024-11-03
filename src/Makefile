NAME = fshell
CFLAGS = -std=c17 $(shell pkg-config --cflags --libs readline) -O2
CC = gcc
HEADERS= array.h base.h exec.h parse.h memory.h alias.h builtin.h fshell.h
SOURCES= base.c exec.c parse.c alias.c builtin.c fshell.c

all:
	${CC} ${HEADERS} ${SOURCES} -o ${NAME} ${CFLAGS}

.PHONY: clean install uninstall debug

clean:
	rm fshell

install:
	cp fshell /bin/
	chmod 755 /bin/fshell

uninstall:
	rm /bin/fshell

debug:
	${CC} ${HEADERS} ${SOURCES} -o ${NAME} -g -pg ${CFLAGS}
