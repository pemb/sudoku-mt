#include "dicas.h"

void *elimina_linha(void *_args)
{
  Args *args = (Args *) _args;
  int i, j, flag = 0, sum, digit;
  for (i = 0; i < 9; i++)
    {
      sum = 0;
      for (j = 0; j < 9; j++)
	{
	  if (args->tabuleiro[args->slice][i][j])
	    {
	      digit = j;
	      sum++;
	    }
	}
      if (sum != 1)
	continue;
      for (j = 0; j < 9; j++)
	{
	  if (j == i)
	    continue;
	  flag = flag || args->tabuleiro[args->slice][j][digit];
	  args->tabuleiro[args->slice][j][digit] = 0;
	}

    }
  return (void *) flag;
}

void *elimina_coluna(void *_args)
{
  Args *args = (Args *) _args;
  int i, j, flag = 0, sum, digit;
  for (i = 0; i < 9; i++)
    {
      sum = 0;
      for (j = 0; j < 9; j++)
	{
	  if (args->tabuleiro[i][args->slice][j])
	    {
	      digit = j;
	      sum++;
	    }
	}
      if (sum == 1)
	{
	  for (j = 0; j < 9; j++)
	    {
	      if (j == i)
		continue;
	      flag = flag || args->tabuleiro[j][args->slice][digit];
	      args->tabuleiro[j][args->slice][digit] = 0;
	    }
	}
    }
  return (void *) flag;
}

void *elimina_quadrado(void *_args)
{
  Args *args = (Args *) _args;
  int i, j, flag = 0, sum, digit;
  for (i = 0; i < 9; i++)
    {
      sum = 0;
      for (j = 0; j < 9; j++)
	{
	  if (args->
	      tabuleiro[(args->slice / 3) * 3 + i / 3][i % 3 +
						       (args->slice % 3) *
						       3][j])
	    {
	      digit = j;
	      sum++;
	    }
	}
      if (sum == 1)
	{
	  for (j = 0; j < 9; j++)
	    {
	      if (j == i)
		continue;
	      flag = flag
		  || args->tabuleiro[(args->slice / 3) * 3 + j / 3][j % 3 +
								    (args->
								     slice
								     % 3) *
								    3]
		  [digit];
	      args->tabuleiro[(args->slice / 3) * 3 + j / 3][j % 3 +
							     (args->slice %
							      3) *
							     3][digit] = 0;
	    }
	}
    }
  return (void *) flag;
}

void *single_linha(void *_args)
{
  Args *args = (Args *) _args;
  int i, j, flag = 0, sum, digit;
  for (i = 0; i < 9; i++)
    {
      sum = 0;
      for (j = 0; j < 9; j++)
	{
	  if (args->tabuleiro[args->slice][j][i])
	    {
	      digit = j;
	      sum++;
	    }
	}
      if (sum != 1)
	continue;
      for (j = 0; j < 9; j++)
	{
	  if (j == i)
	    continue;
	  flag = flag || args->tabuleiro[args->slice][digit][j];
	  args->tabuleiro[args->slice][digit][j] = 0;
	}
    }
  return (void *) flag;
}

void *single_coluna(void *_args)
{
  Args *args = (Args *) _args;
  int i, j, flag = 0, sum, digit;
  for (i = 0; i < 9; i++)
    {
      sum = 0;
      for (j = 0; j < 9; j++)
	{
	  if (args->tabuleiro[j][args->slice][i])
	    {
	      digit = j;
	      sum++;
	    }
	}
      if (sum == 1)
	{
	  for (j = 0; j < 9; j++)
	    {
	      if (j == i)
		continue;
	      flag = flag || args->tabuleiro[digit][args->slice][j];
	      args->tabuleiro[digit][args->slice][j] = 0;
	    }
	}
    }
  return (void *) flag;
}

void *single_quadrado(void *_args)
{
  Args *args = (Args *) _args;
  int i, j, flag = 0, sum, digit;
  for (i = 0; i < 9; i++)
    {
      sum = 0;
      for (j = 0; j < 9; j++)
	{
	  if (args->
	      tabuleiro[(args->slice / 3) * 3 + j / 3][j % 3 +
						       (args->slice % 3) *
						       3][i])
	    {
	      digit = j;
	      sum++;
	    }
	}
      if (sum == 1)
	{
	  for (j = 0; j < 9; j++)
	    {
	      if (j == i)
		continue;
	      flag = flag
		  || args->tabuleiro[(args->slice / 3) * 3 +
				     digit / 3][digit % 3 +
						(args->slice % 3) * 3][j];
	      args->tabuleiro[(args->slice / 3) * 3 +
			      digit / 3][digit % 3 +
					 (args->slice % 3) * 3][j] = 0;
	    }
	}
    }
  return (void *) flag;
}
