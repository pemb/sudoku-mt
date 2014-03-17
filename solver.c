#include "solver.h"
#include "sudoku.h"
#include <pthread.h>
#include <semaphore.h>

#define STRATEGIES 6

/* funções que calculam posição a ser lida */

inline char *remove_row(char ***g, int slice, int index, int pos)
{
  return g[slice][index] + pos;
}

inline char *remove_column(char ***g, int slice, int index, int pos)
{
  return g[index][slice] + pos;
}

inline char *remove_subgrid(char ***g, int slice, int index, int pos)
{
  return
      g
      [(slice / SUBGRID_SIZE) * SUBGRID_SIZE + index / SUBGRID_SIZE]
      [(slice % SUBGRID_SIZE) * SUBGRID_SIZE + index % SUBGRID_SIZE] + pos;
}

inline char *single_row(char ***g, int slice, int index, int pos)
{
  return g[slice][pos] + index;
}

inline char *single_column(char ***g, int slice, int index, int pos)
{
  return g[pos][slice] + index;
}

inline char *single_subgrid(char ***g, int slice, int index, int pos)
{
  return g
      [(slice / SUBGRID_SIZE) * SUBGRID_SIZE + pos / SUBGRID_SIZE]
      [(slice % SUBGRID_SIZE) * SUBGRID_SIZE + pos % SUBGRID_SIZE] + index;
}

/* vetor de funções */
inline char *(*getpos[STRATEGIES]) (char ***, int, int, int) =
{
remove_row, remove_column, remove_subgrid, single_row, single_column,
      single_subgrid};

void *solver_loop(void *_args)
{
  Args *args = (Args *) _args;
  int h, i, j, flag, sum, match, slice;

  /* threads decidem em que fatia do grid vão rodar */

  pthread_mutex_lock(&args->mutex);
  slice = args->slice++;
  pthread_mutex_unlock(&args->mutex);

  while (1)
    {
      /* 1a barreira de sincronização */
      pthread_mutex_lock(&args->mutex);
      if (++(args->count) == GRID_SIZE)
	{
	  sem_wait(args->sem + 1);
	  sem_post(args->sem);
	}

      pthread_mutex_unlock(&args->mutex);

      sem_wait(args->sem);
      sem_post(args->sem);


      flag = 0;
      for (h = 0; h < STRATEGIES; h++)
	{
	  for (i = 0; i < GRID_SIZE; i++)
	    {
	      sum = 0;
	      /* conta ocorrências */
	      for (j = 0; j < GRID_SIZE; j++)
		{
		  if (*getpos[h] (args->tabuleiro, slice, i, j))
		    {
		      match = j;
		      sum++;
		    }
		}
	      if (sum != 1)
		continue;
	      /* se houver só uma, desmarca como possibilidade nos vizinhos e marca a flag de alteração */
	      for (j = 0; j < NUMBERS; j++)
		{
		  if (j == i)
		    continue;
		  flag = flag
		      || *getpos[h] (args->tabuleiro, slice, j, match);
		  *getpos[h] (args->tabuleiro, slice, j, match) = 0;
		}

	    }
	}

      /* if (!args->flag) */
      /*        return NULL; */

      /* 2a barreira de sincronização */
      pthread_mutex_lock(&args->mutex);
      args->flag = flag || args->flag;
      if (!--(args->count))
	{
	  if (!args->flag)
	    {
	      sem_post(args->sem + 2);
	      sem_wait(args->sem + 1);
	    }
	  args->flag = 0;
	  sem_wait(args->sem);
	  sem_post(args->sem + 1);
	}
      pthread_mutex_unlock(&args->mutex);

      sem_wait(args->sem + 1);
      sem_post(args->sem + 1);
    }

  return NULL;
}
