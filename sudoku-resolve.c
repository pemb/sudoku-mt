#include "sudoku.h"
#include "solver.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct TryList
{
  int i, j, possible;
} try_list;

void * resolve_sudoku (void * arg)
{
  char *** tabuleiro = (char ***) arg, *** tmp, ***freeret = NULL;
  Args args;
  int sum, i, j, k, thr_n = 1;
  try_list * tries = NULL;
  
  pthread_t threads[9];
  
  args.tabuleiro = tabuleiro;
  pthread_mutex_init(&args.mutex, NULL);
  for (i = 0; i < 2; i++)
    sem_init (args.sem+i, 0, 0);
  args.flag = args.count = args.slice = 0;

  for (i = 0; i < 9; i++)
    pthread_create(threads+i, NULL, solver_loop, (void *) &args);

  while (i--)
      pthread_join(threads[i], NULL);

  pthread_mutex_destroy(&args.mutex);
  for (i = 0; i < 2; i++)
    sem_destroy(args.sem+i);

  for (i = 0; i < 9; i++)
    {
      for (j = 0; j < 9; j++)
	{
	  sum = 0;
	  for (k = 0; k < 9; k++)
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
	      tries = realloc(tries,sizeof (try_list)*thr_n);
      
	      for (k = 0; k < 9; k++)
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
      for (k = 0; k < 9; k++)
	if (tries[i].possible != k)
	  tmp[tries[i].i][tries[i].j][k] = 0;
      pthread_create(threads+i, NULL, resolve_sudoku, (void *) tmp);
    }

  freetabuleiro(tabuleiro);  
  tabuleiro = NULL;
  free(tries);

  for (i = 0; i < thr_n; i++)
    {
      pthread_join(threads[i], tabuleiro ? &tabuleiro : &freeret);
      freetabuleiro(freeret);
    }
  
  return tabuleiro;
  
  }
			 
int main (void)
{
  char *** tabuleiro = letabuleiro();
  tabuleiro = resolve_sudoku(tabuleiro);
  if (!tabuleiro)
      printf ("no solution\n");
  else
    printtabuleiro(tabuleiro);
  freetabuleiro(tabuleiro);
  return 0;
}

  
    
