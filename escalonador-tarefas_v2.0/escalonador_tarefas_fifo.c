#include "fifo_circular.c"
#include <stdio.h> //para usar o getchar() no lugar do system("pause")

//constantes para setar os MENU's
#define FIRST_MENU 0
#define MAIN_MENU 1

int cont_ciclos = 0;

/*
* Função que gerencia e imprime os MENU's
* Retorno:
* - 1 se o usuario optou por criar uma nova tarefa
*/
int gerenciador_menus(int WHICH_MENU)
{
  int user_choice;

  if (WHICH_MENU == FIRST_MENU)
  {
    //Repete o menu abaixo se o usuario digitar uma opcao invalida
    while (user_choice != 0 && user_choice != 1 && user_choice != 2)
    {
      //MENU
      printf("\n\r===================================MENU TASK===================================\n\r");
      printf("Deseja inserir uma tarefa na fila ? ");
      printf("\n\r- SIM   (Digite 0 e pressione Enter)");
      printf("\n\r- NÃO   (Digite 1 e pressione Enter)");
      printf("\n\r- Sair  (Digite 2 e pressione Enter)");
      scanf("%d", &user_choice);

      //Verifica se o usuario digitou uma opcao invalida
      if (user_choice != 0 && user_choice != 1 && user_choice != 2)
        printf("\n\rOpcao invalida!!!\n\r Pressione Enter.\n\n\r\r");
      getchar();

      //A fila inicia vazia, logo, se o usuario não desejar inserir mais tarefas inicialmente, o programa finaliza
      if (user_choice == 1 || user_choice == 2)
      {
        printf("\n\r\n\rEscalonador finalizado...\n\r");
        return 0;
        exit;
      }

      //Verfica se a opcao escolhida foi criar uma nova tarefa, se sim, retorna 1
      if (user_choice == 0)
        return 1;
    }
  }
  else if (WHICH_MENU == MAIN_MENU)
  {
    //menu principal
  }
}

/*
* Função que simula a utilização do processador pela tarefa.
* A tarefa executa no máximo 10 ciclos por vez. 
* Cada tarefa não pode ultrapassar o total de instruções estabelecido.
* Se a tarefa foi finalizada retorna 1, se não retorna 0
*/
int processa(struct tarefa *p_task)
{
  printf("\n\r===================================PROCESSANDO===================================\n\r");

  //Executa de 1 até 10 ciclos de processamento
  for (int i = 0; i < 10; i++)
  {
    cont_ciclos++;

    printf("Executando a tarefa: %s \n\r", p_task->nome);
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
  //Armazena o número de tarefas atual no escalonador
  int num_tarefas = 0;

  //Checa se o retorno foi diferente de 1, que representa a opcao de criar uma tarefa e se for, finaliza o programa
  if (gerenciador_menus(FIRST_MENU) != 1)
  {
    return 0;
    exit;
  }

  //Cria uma nova tarefa
  struct fila_circular queue = fila_circular_construct(num_tarefas);

  // insere_final(&queue, tarefa_construct("tarefa1", 5));
  // insere_final(&queue, tarefa_construct("tarefa2", 5));
  // insere_final(&queue, tarefa_construct("tarefa3", 5));
  // insere_final(&queue, tarefa_construct("tarefa4", 5));
  // insere_final(&queue, tarefa_construct("tarefa5", 5));

  //Imprimindo a fila cheia
  //imprimir_fila(&queue);

  //Armazena o número de tarefas terminadas
  // int cont_terminadas = 0;

  // //Populando o escalonador com 5 tarefas da fila
  // while (cont_terminadas < num_tarefas)
  // {
  //   //Passando a tarefa a ser processada e verificando se ela foi executada até o final
  //   if (processa(&(queue.x[queue.c])) == 1)
  //   {
  //     remove_inicio(&queue);
  //     cont_terminadas++;
  //   }
  // }

  // //Imprimindo a fila vazia
  // imprimir_fila(&queue);

  // //Imprimindo o número total de ciclos utilizados para processar todas as tarefas da fila
  // printf("\n\r**********************************************************************************");
  // printf("\r\nNumero total de cilos utilizado: %d\n\r", cont_ciclos);
  return 0;
}
