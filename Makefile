CC=gcc
CFLAGS=-Wall -Wextra -O2
LDFLAGS=
SRC=src/adj_list.c src/main.c
OBJ=$(SRC:.c=.o)
BIN=build/markov_part1

all: $(BIN)

build:
	mkdir -p build

$(BIN): build $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ) $(LDFLAGS)

clean:
	rm -f $(OBJ) $(BIN)

.PHONY: all clean
