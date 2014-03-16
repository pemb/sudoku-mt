#include "sudoku.h"
#include "solver.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct TryList {
  int i, j, possible;
} try_list;

void *resolve_sudoku(void *arg)
{
  char ***tabuleiro = (char ***) arg, ***tmp;
  Args args;
  int sum, i, j, k, thr_n = 1;
  try_list *tries = NULL;

  pthread_t threads[GRID_SIZE];

  args.tabuleiro = tabuleiro;
  pthread_mutex_init(&args.mutex, NULL);
  sem_init(args.sem, 0, 0);
  sem_init(args.sem + 1, 0, 1);
  args.flag = args.count = args.slice = 0;

  for (i = 0; i < GRID_SIZE; i++)
    pthread_create(threads + i, NULL, solver_loop, (void *) &args);

  while (i--)
    pthread_join(threads[i], NULL);

  pthread_mutex_destroy(&args.mutex);
  for (i = 0; i < 2; i++)
    sem_destroy(args.sem + i);

  for (i = 0; i < GRID_SIZE; i++)
    {
      for (j = 0; j < GRID_SIZE; j++)
	{
	  sum = 0;
	  for (k = 0; k < NUMBERS; k++)
	    sum += tabuleiro[i][j][k];
	  if (!sum)
	    {
	      free(tries);
	      freetabuleiro(tabuleiro);
	      return NULL;
	    }
	  if (sum > thr_n)
	    {
	      thr_n = sum;
	      tries = realloc(tries, sizeof(try_list) * thr_n);

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
  if (!tries)
    return tabuleiro;

  for (i = 0; i < thr_n; i++)
    {
      tmp = copiatabuleiro(tabuleiro);
      for (k = 0; k < NUMBERS; k++)
	if (tries[i].possible != k)
	  tmp[tries[i].i][tries[i].j][k] = 0;
      tmp = resolve_sudoku(tmp);
      if (tmp)
	{
	  freetabuleiro(tabuleiro);
	  free(tries);
	  return tmp;
	}
    }

  freetabuleiro(tabuleiro);
  free(tries);
  return NULL;
  
}

int main(void)
{
  char ***tabuleiro = letabuleiro();
  tabuleiro = resolve_sudoku(tabuleiro);
  if (!tabuleiro)
    printf("no solution\n");
  else
    printtabuleiro(tabuleiro);
  freetabuleiro(tabuleiro);
  return 0;
}
