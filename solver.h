#ifndef SOLVER_H
#define SOLVER_H

#include <pthread.h>
#include <semaphore.h>

struct _Args {
  char ***tabuleiro, flag, slice, count;
  pthread_mutex_t mutex;
  sem_t sem[2];
};

typedef struct _Args Args;

void *solver_loop(void *);
#endif
