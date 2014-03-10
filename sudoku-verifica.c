#include <pthread.h>
#include <stdio.h>

int tabuleiro[9][9];

void * conferelin( void * arg)
{
  int l = (int) arg, i;
  char existe[9] = 
    {
      0
    };
  for (i = 0; i < 9; i++)
    if (existe[tabuleiro[l][i]-1]++)
      {
	printf ("A linha %d contém duas ocorrências do número %d.\n", l+1, tabuleiro[l][i]);
	break;
      }
  return NULL;
}

void * conferecol( void * arg)
{
  int l = (int) arg, i;
  char existe[9] = 
    {
      0
    };
  for (i = 0; i < 9; i++)
   if (existe[tabuleiro[i][l]-1]++)
      {
	printf ("A coluna %d contém duas ocorrências do número %d.\n", l+1, tabuleiro[i][l]);
	break;
      }
  return NULL;
}

void * conferequa( void * arg)
{
  int l = (int) arg, i;
  char existe[9] = 
    {
      0
    };
  for (i = 0; i < 9; i++)
    if(existe[tabuleiro[(l/3)*3+i/3][i%3+(l%3)*3]-1]++)
      {
	printf ("O quadrado %d contém duas ocorrências do número %d.\n", l+1, tabuleiro[(l/3)*3+i/3][i%3+(l%3)*3]);
	break;
      }
  return NULL;
}

int main( void)
{
  int i, j;
  pthread_t threads[3][9];
  for (i = 0; i < 9; i++)
    for (j = 0; j < 9; j++)
      scanf ("%d",tabuleiro[i]+j);

  for (i = 0; i < 9; i++)
    {
      pthread_create(&threads[0][i], NULL, conferecol, (void *) i);
      pthread_create(&threads[1][i], NULL, conferelin, (void *) i);
      pthread_create(&threads[2][i], NULL, conferequa, (void *) i);
    }

  for (i = 0; i < 3; i++)
    for (j = 0; j < 9; j++)
      pthread_join(threads[i][j], NULL);
  
  return 0;
}
