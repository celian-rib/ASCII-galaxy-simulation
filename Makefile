CFLAGS = --std=c11 -g -Wall -Wextra -pedantic -D_XOPEN_SOURCE=700
LDFLAGS=-lncurses

main: main.o

clean:
	rm -rf *.o main
