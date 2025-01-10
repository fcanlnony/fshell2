CC = gcc
CFLAGS = -std=c17 $(shell pkg-config --cflags --libs readline) -O2 -I./include -D FSHELL_64_BIT
DEBUG = -pg -g -Wall

all:
	$(CC) $(CFLAGS) -c *.c

.PHONY: clean debug

clean:
	rm *.o

debug:
	$(CC) $(CFLAGS) $(DEBUG) -c *.c
