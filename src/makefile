# Makefile

CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lz

all: program

program: main.c archive.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f program
