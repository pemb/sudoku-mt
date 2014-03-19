#include "sudoku.h"
#include "solver.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct TryList {
  int i, j, possible;
} try_list;

pthread_t threads[GRID_SIZE];
Args args;

void *resolve_sudoku(void *arg)
{
  char ***tabuleiro = (char ***) arg, ***tmp;

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
      return NULL;
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
		  tmp = resolve_sudoku(tmp);
		  /* se achou solução, já sai */
		  if (tmp)
		    break;
		}
	      freetabuleiro(tabuleiro);
	      return tmp;
	    }
	}
    }

  /* se não houve mais de uma possibilidade */
  return tabuleiro;
}

int main(void)
{
  int i;
  /* lê tabuleiro */
  char ***tabuleiro = letabuleiro();

  /* inicializa primitivas de sincronização */
  pthread_mutex_init(&args.mutex, NULL);
  for (i = 0; i < 3; i++)
    sem_init(args.sem + i, 0, 0);

  args.flag = args.count = args.slice = 0;

  /* abre threads */
  for (i = 0; i < GRID_SIZE; i++)
    pthread_create(threads + i, NULL, solver_loop, (void *) &args);

  /* chama função que resolve */
  tabuleiro = resolve_sudoku(tabuleiro);

  /* while (i--) */
  /*   pthread_join(threads[i], NULL); */

  /* destrói primitivas de sincronização */
  pthread_mutex_destroy(&args.mutex);
  for (i = 0; i < 2; i++)
    sem_destroy(args.sem + i);

  /*imprime resultado */
  if (!tabuleiro)
    printf("no solution\n");
  else
    printtabuleiro(tabuleiro);

  /* libera memória */
  freetabuleiro(tabuleiro);
  return 0;
}
