#include "linked_list.c"

/*
* Função que simula a utilização do processador pela tarefa.
* A tarefa executa no máximo 10 ciclos por vez. 
* Cada tarefa não pode ultrapassar o total de instruções estabelecido.
* Se a tarefa foi finalizada retorna 1, se não retorna 0
* Tarefas não finalizadas em 10 ciclos são recolocadas para o final da fila
*/
int process(Task *p_task)
{
  printf("\n\r===================================PROCESSANDO===================================\n\r");

  //Executa de 1 até 10 ciclos de processamento
  for (int i = 0; i < 10; i++)
  {
    printf("Executando a tarefa: %s \n\r", p_task->name);
    p_task->counter++; //Incrementando o contador de ciclos

    //Verificando se a tarefa executou até o final p/ retornar 1
    if (p_task->counter == p_task->total_instructions)
      return 1;
  }

  //Se não executou até o final retorna 0
  return 0;
}

//Processando as tarefas da fila
int main()
{
  Queue q = queue_construct();
  insert_end(&q, task_construct("tarefa1", 15));
  insert_end(&q, task_construct("tarefa2", 10));
  insert_end(&q, task_construct("tarefa3", 12));
  insert_end(&q, task_construct("tarefa4", 10));
  insert_end(&q, task_construct("tarefa5", 10));

  //Imprimindo a fila cheia
  print_queue(&q);

  //Armazena o número de tarefas terminadas
  int count_finished = 0;

  //Populando o escalonador com 5 tarefas da fila
  while (q.qtd_nos > 0)
  {
    // printf("\n\rN tarefas terminadas %d \n\rN quantidade de tarefas: %d\n\r", count_finished, q.qtd_nos);

    //Passando a tarefa a ser processada e verificando se ela foi executada até o final
    if (process(&(q.first_no->info)) == 1)
    {
      remove_begin(&q);
      count_finished++;
    }
    //Se a tarefa não foi processada até o final ela volta para o final da fila
    else
    {
      //Tirando a tarefa do inicio a passando para o final
      from_begin_to_end(&q);
    }
  }

  //Imprimindo a fila vazia
  // print_queue(&q);
  return 0;
}
