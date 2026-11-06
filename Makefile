CC = gcc
CFLAGS = -Wall -O3

all: cibve

cibve: main.c
	$(CC) $(CFLAGS) main.c -o cibve

clean:
	rm -f cibve
