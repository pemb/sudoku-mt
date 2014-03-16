#include "sudoku.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *** copiatabuleiro(char *** old)
{
  int i, j;
  char *** tabuleiro = malloc(9*sizeof(char **));
  tabuleiro[0] = malloc(9*9*sizeof(char *));
  tabuleiro[0][0] = malloc(9*9*9*sizeof(char));
  if (old)
    memcpy(tabuleiro[0][0], **old, 9*9*9*sizeof(char));
  else
    memset(tabuleiro[0][0], 1, 9*9*9*sizeof(char));
  for (i = 0; i < 9; i++)
    {
      if (i)
	{
	  tabuleiro[i] = tabuleiro[i-1]+9;
	  tabuleiro[i][0] = tabuleiro[i-1][8]+9;
	}
      for (j = 1; j < 9; j++)
	tabuleiro[i][j] = tabuleiro[i][j-1]+9;
    }
  return tabuleiro;
}

char *** letabuleiro(void)
{
  char *** tabuleiro = copiatabuleiro(NULL), read;
  int i, j, k;
  for (i = 0; i < 9; i++)
    for (j = 0; j < 9; j++)
      {
	scanf (" %c", &read);
	  if (read > '0' && read <= '9')
	    for (k = 0; k < 9; k++)
	      tabuleiro[i][j][k] = k == (read - '1');
      }
  return tabuleiro;
}

char *** letabuleiro_202(void)
{
  char *** tabuleiro = copiatabuleiro(NULL);
  int n, i, j, k, d;
  scanf ("%d", &n);
  while (n--)
    {
      scanf ("%d %d %d", &i, &j, &d);
      for (k = 0; k < 9; k++)
	tabuleiro[i-1][j-1][k] = k == (d - 1);
    }
  return tabuleiro;
}


void freetabuleiro(char *** tab)
{
  if (!tab)
    return;
  free(tab[0][0]);
  free(tab[0]);
  free(tab);
}

void printtabuleiro(char *** tabuleiro)
{
  int i, j, k;
  char sum, digit;
    for (i = 0; i < 9; i++)
    {
      for (j = 0; j < 9; j++)
	{
	  sum = 0;
	  for (k = 0; k < 9; k++)
	    {
	      if (tabuleiro[i][j][k])
		{
		  sum++;
		  digit = k;
		}
	    }
	  if (sum == 1)
	    printf ("%d ", digit + 1);
	  else
	    {
	      putchar('(');
	      if (sum == 0)
	    	putchar('N');
	      for (k = 0; k < 9; k++)
	    	if (tabuleiro[i][j][k])
	    	  putchar ('1'+k);
	      printf (") ");
	    }
	}
      putchar('\n');
    }
}
