#include "sudoku.h"
#include "dicas.h"
#include <pthread.h>
#include <stdio.h>

void * resolve_sudoku (void * arg)
{
  char *** tabuleiro = (char ***) arg, ***tmp;
  Args args[9];
  void * ret = NULL;
  int flag = 1, i, j, k, l;
  pthread_t threads[9];
  void * (*elimina[]) (void *) = {elimina_linha, elimina_coluna, elimina_quadrado, single_linha, single_coluna, single_quadrado};

  for (i = 0; i < 9; i++)
    {
      args[i].tabuleiro = tabuleiro;
      args[i].slice = i;
    }
  while (flag)
    {
      flag = 0;
      for (i = 0; i < 6; i++)
	{
	  for (j = 0; j < 9; j++)
	    pthread_create(threads+j, NULL, elimina[i], args+j);
	  for (j = 0; j < 9; j++)
	    {
	      pthread_join(threads[j], &ret);
	      if (ret)
		flag = 1;
	    }
	}
    }
  for (i = 0; i < 9; i++)
    {
      for (j = 0; j < 9; j++)
	{
	  flag = 0;
	  for (k = 0; k < 9; k++)
	    flag += tabuleiro[i][j][k];
	  if (!flag)
	    {
	      freetabuleiro(tabuleiro);
	      return NULL;
	    }
	  if (flag == 1)
	    continue;
	  for (k = 0; k < 9; k++)
	    {
	      if (tabuleiro[i][j][k])
		{
		  tmp = copiatabuleiro(tabuleiro);
		  for (l = 0; l < 9; l++)
		    if (l != k)
		      tmp[i][j][l] = 0;
		  pthread_create(&threads[k], NULL, resolve_sudoku, (void *) tmp);
		}
	    }
	  for (k = 0; k < 9; k++)
	    if (tabuleiro[i][j][k])
	      pthread_join(threads[k], ret ? NULL : &ret);
	  freetabuleiro(tabuleiro);
	  return ret;

	}
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
  return 0;
}
  
    
