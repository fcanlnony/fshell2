NAME=fshell
CFLAGS = -std=c17 $(shell pkg-config --cflags --libs readline) -O2
CC = gcc
DEBUG = -pg -g -Wall

$(NAME):
	$(CC) $(CFLAGS) -I./ -c *.c
	$(CC) $(CFLAGS) *.o -o $@

.PHONY: clean install uninstall debug

clean:
	rm fshell *.o

install:
	cp fshell /bin/
	chmod 755 /bin/fshell

uninstall:
	rm /bin/fshell

debug:
	$(CC) $(CFLAGS) $(DEBUG) -I./ -c *.c
	$(CC) $(CFLAGS) *.o -o $(NAME)

