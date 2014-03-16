#ifndef SUDOKU_H
#define SUDOKU_H

#define GRID_SIZE 9
#define SUBGRID_SIZE 3
#define NUMBERS 9

char ***copiatabuleiro(char ***);

char ***letabuleiro(void);

char ***letabuleiro_202(void);

void freetabuleiro(char ***);

void printtabuleiro(char ***);

#endif
