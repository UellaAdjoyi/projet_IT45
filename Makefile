#CC = gcc
#CFLAGS = -Wall -Wextra -O2
#SRC = src
#BIN = build
#
#all: tsp_solver
#
#tsp_solver: $(SRC)/main.c $(SRC)/little.c $(SRC)/tabu.c $(SRC)/genetic.c
#	$(CC) $(CFLAGS) -o $(BIN)/tsp_solver \
#		$(SRC)/main.c $(SRC)/little.c $(SRC)/tabu.c $(SRC)/genetic.c
#
#clean:
#	rm -f $(BIN)/*
# Makefile
CC = gcc
CFLAGS = -Wall -O2
OBJS = main.o little.o
EXEC = tsp_little

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS) -lm

main.o: main.c little.h
	$(CC) $(CFLAGS) -c main.c

little.o: little.c little.h
	$(CC) $(CFLAGS) -c little.c

clean:
	rm -f *.o $(EXEC)
