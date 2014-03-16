#include "sudoku.h"
#include "solver.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct TryList {
  int i, j, possible;
} try_list;

pthread_t threads[GRID_SIZE];
Args args;

void *resolve_sudoku(void *arg)
{
  char ***tabuleiro = (char ***) arg, ***tmp;

  int sum, i, j, k, branches = 1;
  try_list *tries = NULL;

  /* grava apontador para o grid a ser resolvido */

  args.tabuleiro = tabuleiro;

  /* libera threads para trabalharem e espera fim da eliminação */

  sem_post(args.sem+1);
  sem_wait(args.sem+2);
  
  /* para cada célula do grid */
  
  for (i = 0; i < GRID_SIZE; i++)
    {
      for (j = 0; j < GRID_SIZE; j++)
	{
	  /* conta possibilidades pra cada célula */
	  sum = 0;
	  for (k = 0; k < NUMBERS; k++)
	    sum += tabuleiro[i][j][k];

	  /* se não houver possibilidade, não há solução para o grid */
	  if (!sum)
	    {
	      free(tries);
	      freetabuleiro(tabuleiro);
	      return NULL;
	    }
	  
	  /* se há mais do que 1 (pega a maior no fim das contas), preparar
	     para entrar no backtracking */

	  if (sum > branches)
	    {
	      /* aloca memória pra guardar as possibilidades */
	      branches = sum;
	      tries = realloc(tries, sizeof(try_list) * branches);

	      /* grava posição e possibilidades */
	      for (k = 0; k < NUMBERS; k++)
		{
		  if (tabuleiro[i][j][k])
		    {
		      tries[--sum].i = i;
		      tries[sum].j = j;
		      tries[sum].possible = k;
		    }
		}
	    }
	}
    }

  /* se não houve mais de uma possibilidade*/
  if (!tries)
    return tabuleiro;


  for (i = 0; i < branches; i++)
    {
      /* copia trabalho */
      tmp = copiatabuleiro(tabuleiro);
      /* chuta um número possível */
      for (k = 0; k < NUMBERS; k++)
	if (tries[i].possible != k)
	  tmp[tries[i].i][tries[i].j][k] = 0;
      /* chama a recursão */
      tmp = resolve_sudoku(tmp);
      /* se achou solução, já sai */
      if (tmp)
	break;
	/* { */
	/*   freetabuleiro(tabuleiro); */
	/*   free(tries); */
	/*   return tmp; */
	/* } */
    }

  freetabuleiro(tabuleiro);
  free(tries);
  return tmp;
}

int main(void)
{
  int i;
  char ***tabuleiro = letabuleiro();

  pthread_mutex_init(&args.mutex, NULL);
  for (i = 0; i < 3; i++)
    sem_init(args.sem+i, 0, 0);
  
  args.flag = args.count = args.slice = 0;

  for (i = 0; i < GRID_SIZE; i++)
    pthread_create(threads + i, NULL, solver_loop, (void *) &args);

  tabuleiro = resolve_sudoku(tabuleiro);

  /* while (i--) */
  /*   pthread_join(threads[i], NULL); */

  pthread_mutex_destroy(&args.mutex);
  for (i = 0; i < 2; i++)
    sem_destroy(args.sem + i);

  if (!tabuleiro)
    printf("no solution\n");
  else
    printtabuleiro(tabuleiro);
  freetabuleiro(tabuleiro);
  return 0;
}
