#include "fifo_circular.c"

#define N_TAREFAS 5

/*
* Função que simula a utilização do processador pela tarefa.
* A tarefa executa no máximo 10 ciclos por vez. 
* Cada tarefa não pode ultrapassar o total de instruções estabelecido.
* Se a tarefa foi finalizada retorna 1, se não retorna 0
*/
int processa(struct tarefa *p_task)
{
  printf("\n\r\t\t\t\t\t===================================PROCESSANDO===================================\n\r");

  //Executa de 1 até 10 ciclos de processamento
  for (int i = 0; i < 10; i++)
  {
    printf("\t\t\t\t\tExecutando a tarefa: %s \n\r", p_task->nome);
    p_task->contador++; //Incrementando o contador de ciclos

    //Verificando se a tarefa executou até o final p/ retornar 1
    if (p_task->contador == p_task->total_instrucoes)
      return 1;
  }

  //Se não executou até o final retorna 0
  return 0;
}

//Processando as tarefas da fila
int main()
{
  struct fila_circular queue = fila_circular_construct(N_TAREFAS);
  insere_final(&queue, tarefa_construct("tarefa1", 5));
  insere_final(&queue, tarefa_construct("tarefa2", 5));
  insere_final(&queue, tarefa_construct("tarefa3", 5));
  insere_final(&queue, tarefa_construct("tarefa4", 5));
  insere_final(&queue, tarefa_construct("tarefa5", 5));

  //Imprimindo a fila cheia
  imprimir_fila(&queue);

  //Armazena o número de tarefas terminadas
  int cont_terminadas = 0;

  //Populando o escalonador com 5 tarefas da fila
  while (cont_terminadas < N_TAREFAS)
  {
    //Passando a tarefa a ser processada e verificando se ela foi executada até o final
    if (processa(&(queue.x[queue.c])) == 1)
    {
      remove_inicio(&queue);
      cont_terminadas++;
    }
  }

  //Imprimindo a fila vazia
  imprimir_fila(&queue);
  return 0;
}
