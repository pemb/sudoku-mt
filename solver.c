#include "solver.h"
#include "sudoku.h"
#include <pthread.h>
#include <semaphore.h>

#define STRATEGIES 6

char * remove_row(char *** g, int slice, int index, int pos)
{
  return g[slice][index]+pos;
}

char * remove_column(char *** g, int slice, int index, int pos)
{
  return g[index][slice]+pos;
}

char * remove_subgrid(char *** g, int slice, int index, int pos)
{
  return 
    g
    [(slice / SUBGRID_SIZE) * SUBGRID_SIZE + index / SUBGRID_SIZE]
    [(slice % SUBGRID_SIZE) * SUBGRID_SIZE + index % SUBGRID_SIZE]
    +pos;
}

char * single_row(char *** g, int slice, int index, int pos)
{
  return g[slice][pos]+index;
}

char * single_column(char *** g, int slice, int index, int pos)
{
  return g[pos][slice]+index;
}

char * single_subgrid(char *** g, int slice, int index, int pos)
{
  return g
    [(slice / SUBGRID_SIZE) * SUBGRID_SIZE + pos / SUBGRID_SIZE]
    [(slice % SUBGRID_SIZE) * SUBGRID_SIZE + pos % SUBGRID_SIZE]
    +index;
}

char * (*getpos[STRATEGIES]) (char ***, int, int, int) = 
{
  remove_row, remove_column, remove_subgrid, single_row, single_column, single_subgrid
};

void *solver_loop(void *_args)
{
  Args *args = (Args *) _args;
  int h, i, j, flag, sum, match, slice;

  pthread_mutex_lock(&args->mutex);
  slice = args->slice++;
  pthread_mutex_unlock(&args->mutex);

  
  while (1)
    {
      flag = 0;
      for (h = 0; h < STRATEGIES; h++)
	{
	  for (i = 0; i < GRID_SIZE; i++)
	    {
	      sum = 0;
	      for (j = 0; j < GRID_SIZE; j++)
		{
		  if (*getpos[h](args->tabuleiro, slice, i, j))
		    {
		      match = j;
		      sum++;
		    }
		}
	      if (sum != 1)
		continue;
	      for (j = 0; j < NUMBERS; j++)
		{
		  if (j == i)
		    continue;
		  flag = flag || *getpos[h](args->tabuleiro,slice,j,match);
		  *getpos[h](args->tabuleiro,slice,j,match) = 0;
		}

	    }
	}

      pthread_mutex_lock(&args->mutex);
      if (++(args->count) == GRID_SIZE)
      	{
      	  sem_wait(args->sem + 1);
      	  sem_post(args->sem);
      	}
      args->flag = flag || args->flag;
      pthread_mutex_unlock(&args->mutex);

      sem_wait(args->sem);
      sem_post(args->sem);

      if (!args->flag)
      	return NULL;

      pthread_mutex_lock(&args->mutex);
      if (!--(args->count))
      	{
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
