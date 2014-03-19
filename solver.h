#ifndef SOLVER_H
#define SOLVER_H

#define UNSOLVED 0x0
#define SOLVED 0x1
#define IMPOSSIBLE 0x10

#include <pthread.h>
#include <semaphore.h>

struct _Args {
  char ***tabuleiro;
  int flag, slice, count;
  pthread_mutex_t mutex;
  sem_t sem[3];
};

typedef struct _Args Args;

void *solver_loop(void *);
#endif
