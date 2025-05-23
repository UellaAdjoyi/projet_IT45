CC = gcc
CFLAGS = -Wall -Wextra -O2
SRC = src
BIN = build

all: tsp_solver

tsp_solver: $(SRC)/main.c $(SRC)/little.c $(SRC)/tabu.c $(SRC)/genetic.c
	$(CC) $(CFLAGS) -o $(BIN)/tsp_solver \
		$(SRC)/main.c $(SRC)/little.c $(SRC)/tabu.c $(SRC)/genetic.c

clean:
	rm -f $(BIN)/*
