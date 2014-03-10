#include "dicas.h"
#include "sudoku.h"

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int main()
{
  char *** tabuleiro;
  int i, j;
  pthread_t threads[9];
  void * (*elimina[]) (void *) = {elimina_linha, elimina_coluna, elimina_quadrado};
  Args args[9];
  tabuleiro = letabuleiro();
  
  for (i = 0; i < 9; i++)
    {
      args[i].tabuleiro = tabuleiro;
      args[i].slice = i;
    }

  for (i = 0; i < 3; i++)
    {
      for (j = 0; j < 9; j++)
  	pthread_create(threads+j, NULL, elimina[i], args+j);
      for (j = 0; j < 9; j++)
  	pthread_join(threads[j], NULL);
    }

  printtabuleiro(tabuleiro);
  
  freetabuleiro(tabuleiro);

  return 0;
}
