#include "sudoku.h"
#include "solver.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
typedef struct TryList {
  int i, j, possible;
} try_list;

pthread_t threads[GRID_SIZE];
Args args;

uint64_t resolve_sudoku(void *arg)
{
  char ***tabuleiro = (char ***) arg, ***tmp;
  uint64_t count = 0;
  int i, j, k, branches;
  try_list *tries = NULL;

  /* grava apontador para o grid a ser resolvido */

  args.tabuleiro = tabuleiro;

  /* libera threads para trabalharem e espera fim da eliminação */

  sem_post(args.sem + 1);
  sem_wait(args.sem + 2);

  if (args.flag & IMPOSSIBLE)
    {
      free(tries);
      freetabuleiro(tabuleiro);
      /* return NULL; */
      return 0;
    }

  /* para cada célula do grid */

  for (i = 0; i < GRID_SIZE; i++)
    {
      for (j = 0; j < GRID_SIZE; j++)
	{
	  /* conta possibilidades pra cada célula */
	  branches = 0;
	  for (k = 0; k < NUMBERS; k++)
	    branches += tabuleiro[i][j][k];

	  /* se há mais do que 1 (pega a maior no fim das contas), preparar
	     para entrar no backtracking */
	  if (!branches)
	    fprintf (stderr, "WTF");

	  if (branches > 1)
	    {
	      for (branches = 0; branches < NUMBERS; branches++)
		{
		  if (!tabuleiro[i][j][branches])
		    continue;
		  /* copia trabalho */
		  tmp = copiatabuleiro(tabuleiro);
		  /* chuta um número possível */
		  for (k = 0; k < NUMBERS; k++)
		    if (branches != k)
		      tmp[i][j][k] = 0;
		  /* chama a recursão */
		  count += resolve_sudoku(tmp);
		  /* se achou solução, já sai */
		  /* if (tmp) */
		  /*   break; */
		}
	      freetabuleiro(tabuleiro);
	      return count;
	    }
	}
    }
  freetabuleiro(tabuleiro);
  /* se não houve mais de uma possibilidade */
  return 1;
}

int main(void)
{
  int i;
  /* lê tabuleiro */
  char ***tabuleiro = letabuleiro();
  uint64_t count;

  /* inicializa primitivas de sincronização */
  pthread_mutex_init(&args.mutex, NULL);
  for (i = 0; i < 3; i++)
    sem_init(args.sem + i, 0, 0);

  args.flag = args.count = args.slice = 0;

  /* abre threads */
  for (i = 0; i < GRID_SIZE; i++)
    pthread_create(threads + i, NULL, solver_loop, (void *) &args);

  /* chama função que resolve */
  count = resolve_sudoku(tabuleiro);

  args.tabuleiro = NULL;
  sem_post(args.sem+1);

  for (i = 0; i < GRID_SIZE; i++)
    pthread_join(threads[i], NULL);

  /* destrói primitivas de sincronização */
  pthread_mutex_destroy(&args.mutex);
  for (i = 0; i < 2; i++)
    sem_destroy(args.sem + i);

  /*imprime resultado */
  /* if (!tabuleiro) */
  /*   printf("no solution\n"); */
  /* else */
  /*   printtabuleiro(tabuleiro); */
  
  printf ("%lu\n", count);

  /* libera memória */
  /* freetabuleiro(tabuleiro); */
  return 0;
}
