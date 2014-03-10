CFLAGS= -g -Og -pthread -Wall -pedantic

BINS= sudoku-verifica sudoku-dicas sudoku-resolve

all: $(BINS)

sudoku-dicas: dicas.o sudoku.o

sudoku-resolve: dicas.o sudoku.o

.PHONY: clean

clean:
	rm -f *.o $(BINS)
