#include "solver.h"
#include <pthread.h>
#include <semaphore.h>

void * solver_loop(void * _args)
{
  Args * args = (Args *) _args;
  int i,j, flag, sum, match, slice;

  pthread_mutex_lock(&args->mutex);
  slice = args->slice++;
  pthread_mutex_unlock(&args->mutex);

  do
    {
      flag = 0;
      for (i = 0; i < 9; i++)
	{
	  sum = 0;
	  for (j = 0; j < 9; j++)
	    {
	      if (args->tabuleiro[slice][i][j])
		{
		  match = j;
		  sum++;
		}
	    }
	  if (sum != 1)
	    continue;
	  for (j = 0; j < 9; j++)
	    {
	      if (j == i)
		continue;
	      flag = flag || args->tabuleiro[slice][j][match];
	      args->tabuleiro[slice][j][match] = 0;
	    }

	}

      for (i = 0; i < 9; i++)
	{
	  sum = 0;
	  for (j = 0; j < 9; j++)
	    {
	      if (args->tabuleiro[slice][j][i])
		{
		  match = j;
		  sum++;
		}
	    }
	  if (sum != 1)
	    continue;
	  for (j = 0; j < 9; j++)
	    {
	      if (j == i)
		continue;
	      flag = flag || args->tabuleiro[slice][match][j];
	      args->tabuleiro[slice][match][j] = 0;
	    }
	}

      pthread_mutex_lock(&args->mutex);
      if (++(args->count) == 9)
	sem_post(args->sem);      
      pthread_mutex_unlock(&args->mutex);

      sem_wait(args->sem);
      sem_post(args->sem);
      
      pthread_mutex_lock(&args->mutex);
      flag = args->flag;      
      if(!--(args->count))
	{
	  sem_post(args->sem+1);
	}
      pthread_mutex_unlock(&args->mutex);

      sem_wait(args->sem+1);
      sem_post(args->sem+1);
  
      for (i = 0; i < 9; i++)
	{
	  sum = 0;
	  for (j = 0; j < 9; j++)
	    {
	      if (args->tabuleiro[i][slice][j])
		{
		  match = j;
		  sum++;
		}
	    }
	  if (sum == 1)
	    {
	      for (j = 0; j < 9; j++)
		{
		  if (j == i)
		    continue;
		  flag = flag || args->tabuleiro[j][slice][match];
		  args->tabuleiro[j][slice][match] = 0;
		}
	    }
	}

	
      for (i = 0; i < 9; i++)
	{
	  sum = 0;
	  for (j = 0; j < 9; j++)
	    {
	      if (args->tabuleiro[j][slice][i])
		{
		  match = j;
		  sum++;
		}
	    }
	  if (sum == 1)
	    {
	      for (j = 0; j < 9; j++)
		{
		  if (j == i)
		    continue;
		  flag = flag || args->tabuleiro[match][slice][j];
		  args->tabuleiro[match][slice][j] = 0;
		}
	    }
	}

      pthread_mutex_lock(&args->mutex);
      if (++(args->count) == 9)
	sem_post(args->sem);      
      pthread_mutex_unlock(&args->mutex);

      sem_wait(args->sem);
      sem_post(args->sem);
      
      pthread_mutex_lock(&args->mutex);
      flag = args->flag;      
      if(!--(args->count))
	{
	  sem_post(args->sem+1);
	}
      pthread_mutex_unlock(&args->mutex);

      sem_wait(args->sem+1);
      sem_post(args->sem+1);


      for (i = 0; i < 9; i++)
	{
	  sum = 0;
	  for (j = 0; j < 9; j++)
	    {
	      if (args->tabuleiro[(slice/3)*3+i/3][i%3+(slice%3)*3][j])
		{
		  match = j;
		  sum++;
		}
	    }
	  if (sum == 1)
	    {
	      for (j = 0; j < 9; j++)
		{
		  if (j == i)
		    continue;
		  flag = flag || args->tabuleiro[(slice/3)*3+j/3][j%3+(slice%3)*3][match];
		  args->tabuleiro[(slice/3)*3+j/3][j%3+(slice%3)*3][match] = 0;
		}
	    }
	}

      for (i = 0; i < 9; i++)
	{
	  sum = 0;
	  for (j = 0; j < 9; j++)
	    {
	      if (args->tabuleiro[(slice/3)*3+j/3][j%3+(slice%3)*3][i])
		{
		  match = j;
		  sum++;
		}
	    }
	  if (sum == 1)
	    {
	      for (j = 0; j < 9; j++)
		{
		  if (j == i)
		    continue;
		  flag = flag || args->tabuleiro[(slice/3)*3+match/3][match%3+(slice%3)*3][j];
		  args->tabuleiro[(slice/3)*3+match/3][match%3+(slice%3)*3][j] = 0;
		}
	    }
	}

      pthread_mutex_lock(&args->mutex);
      if (++(args->count) == 9)
	sem_post(args->sem);      
      args->flag = flag || args->flag;
      pthread_mutex_unlock(&args->mutex);

      sem_wait(args->sem);
      sem_post(args->sem);
      
      pthread_mutex_lock(&args->mutex);
      flag = args->flag;      
      if(!--(args->count))
	{
	  args->flag = 0;
	  sem_post(args->sem+1);
	}
      pthread_mutex_unlock(&args->mutex);

      sem_wait(args->sem+1);
      sem_post(args->sem+1);
    }
  while (flag);
  

  return NULL;
}
