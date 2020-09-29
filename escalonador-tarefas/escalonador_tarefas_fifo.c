#ifndef struct fila_circular
#include "fifo_circular.c"
#endif
int main()
{
  struct fila_circular p_queue = fila_circular_construct(4);

  insere_final(&p_queue, 10);
  insere_final(&p_queue, 1);
  insere_final(&p_queue, 11);
  insere_final(&p_queue, 12);

  remove_inicio(&p_queue);

  imprimir_fila(&p_queue);
  return 0;
}