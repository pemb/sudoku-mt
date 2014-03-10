struct _Args
{
  char *** tabuleiro;
  int slice;
};

typedef struct _Args Args;

void * elimina_linha(void *);

void * elimina_coluna(void *);

void * elimina_quadrado(void *);

void * single_linha(void *);

void * single_coluna(void *);

void * single_quadrado(void *);

