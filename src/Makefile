NAME=fshell
CFLAGS = -std=c17 $(shell pkg-config --cflags --libs readline) -O2 -I./ -I./include -I./config -D FSHELL_64_BIT
CC = gcc
DEBUG = -pg -g -Wall
LINK = lib/*.o config/*.o

$(NAME):
	$(CC) $(CFLAGS) -c *.c
	make -C lib
	make -C config
	$(CC) $(CFLAGS) *.o ${LINK} -o $@

.PHONY: clean install uninstall debug

clean:
	rm fshell *.o
	make clean -C lib
	make clean -C config

install:
	cp fshell /bin/
	chmod 755 /bin/fshell

uninstall:
	rm /bin/fshell

debug:
	$(CC) $(CFLAGS) $(DEBUG) -c *.c
	make debug -C lib
	make debug -C config
	$(CC) $(CFLAGS) *.o ${LINK} -o $(NAME)

