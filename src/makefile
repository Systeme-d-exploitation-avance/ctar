# Makefile

CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lz  # Ajoute l'option -lz pour inclure la bibliothèque zlib

all: program

program: main.c archive.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f program
