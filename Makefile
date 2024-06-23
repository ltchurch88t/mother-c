CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11
LDFLAGS = -lncurses

SRC = main.c splash.c matrix.c directives.c execute_order.c journal.c access_info.c screen_helpers.c helpers.c config.c
OBJ = $(SRC:.c=.o)
TARGET = alien-console

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean

