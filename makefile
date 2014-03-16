CFLAGS= -g -O0 -pthread -Wall -pedantic

BINS= sudoku-verifica sudoku-dicas sudoku-resolve

all: $(BINS)

sudoku-dicas: dicas.o sudoku.o

sudoku-resolve: solver.o sudoku.o

.PHONY: clean

clean:
	rm -f *.o $(BINS)
