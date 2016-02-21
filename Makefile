PROG=test
CC=gcc
CFLAGS=-g -W -Wall

all: main.c
	$(CC) $(CFLAGS) main.c -o $(PROG)
