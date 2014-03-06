#include <pthread.h>
#include <stdio.h>

int tabuleiro[9][9];

void * conferelinha( void * arg)
{
  int l = (int) arg, i;
  char existe[9] = 
    {
      0
    };
  for (i = 0; i < 9; i++)
    existe[tabuleiro[l][i]-1]++;
  for (i = 0; i < 9; i++)
    if (existe[i] != 1)
      return (void *) 1;
  return (void *) 0;
}

void * conferecol( void * arg)
{
  int l = (int) arg, i;
  char existe[9] = 
    {
      0
    };
  for (i = 0; i < 9; i++)
    existe[tabuleiro[i][l]-1]++;
  for (i = 0; i < 9; i++)
    if (existe[i] != 1)
      return (void *) 1;
  return (void *) 0;
}

void * conferequad( void * arg)
{
  int l = (int) arg, i;
  char existe[9] = 
    {
      0
    };
  for (i = 0; i < 9; i++)
    existe[tabuleiro[(l/3)*3+i/3][i%3+(l%3)*3]-1]++;
  for (i = 0; i < 9; i++)
    if (existe[i] != 1)
      return (void *) 1;
  return (void *) 0;
}


int main( void)
{
  int i, j;
  void * ret;
  pthread_t threads[27];
  for (i = 0; i < 9; i++)
    for (j = 0; j < 9; j++)
      scanf ("%d",tabuleiro[i]+j);

  for (i = 0; i < 9; i++)
    {
      pthread_create(threads+i, NULL, conferecol, (void *) i);
      pthread_create(threads+i+9, NULL, conferelinha, (void *) i);
      pthread_create(threads+i+18, NULL, conferequad, (void *) i);
    }

  for (i = 0; i < 27; i++)
    {
      pthread_join(threads[i], &ret);
      if ((int) ret)
	{
	  printf ("fail\n");
	  return 0;
	}
    }
  
  printf ("success\n");
  

  /* for (i = 0; i < 9; i++) */
  /*   { */
  /*     for (j = 0; j < 9; j++) */
  /* 	printf ("%d ", tabuleiro[i][j]); */
  /*     putchar('\n'); */
  /*   } */
  
  return 0;
}
