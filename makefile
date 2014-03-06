CFLAGS= -g -pthread

BINS= sudoku-verifica

all: $(BINS)

.PHONY: clean

clean:
	rm -f *.o $(BINS)
