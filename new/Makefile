CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11
LDFLAGS = -lncurses

OBJ = main.o splash.o matrix.o directives.o execute_order.o journal.o access_info.o screen_helpers.o helpers.o

all: main

main: $(OBJ)
	$(CC) -o main $(OBJ) $(LDFLAGS)

%.o: %.c alien-console.h
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f *.o main

