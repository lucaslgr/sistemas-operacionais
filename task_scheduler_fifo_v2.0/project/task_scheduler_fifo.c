#include "linked_list.c"

//constantes para setar os MENU's
#define FIRST_MENU 0
#define MAIN_MENU 1

int count_cycles = 0;

void system_pause()
{
  // char command[100];
  // int i;
  // scanf("%s", command);
  // printf("Presione ENTER para continuar...");
  // while (getchar() != '\n')
  // {
  //   i = 0;
  // }
  // getchar();
  // return;
  int command = -1;
  while (command != 0)
  {
    printf("\n\rPresione 0 e posteriormente ENTER para continuar: ");
    scanf("%d", &command);
  }
  return;
}

/*
* Função que gerencia e imprime os MENU's
* Retorno:
* - 1 se o usuario optou por criar uma nova tarefa
* - 0 se optou por processar uma tarefa
*/
int manager_menus(int WHICH_MENU)
{
  system("clear");
  int user_choice = -1;
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
      printf("\n\r- Sair  (Digite 2 e pressione Enter)\n\r");
      scanf("%d", &user_choice);

      //Verifica se o usuario digitou uma opcao invalida
      if (user_choice != 0 && user_choice != 1 && user_choice != 2)
      {
        printf("\n\rOpcao invalida!!!\n\r Pressione Enter.\n\n\r\r");
        system_pause();
      }

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
    //Repete o menu abaixo se o usuario digitar uma opcao invalida
    while (user_choice != 0 && user_choice != 1 && user_choice != 2)
    {
      //MENU
      printf("\n\r===================================MENU TASK===================================\n\r");
      printf("\n\r- Deseja inserir outra tarefa na fila? (Digite 0 e pressione Enter)");
      printf("\n\r- Deseja processar uma tarefa da fila? (Digite 1 e pressione Enter)");
      printf("\n\r- Sair  (Digite 2 e pressione Enter)\n\r");
      scanf("%d", &user_choice);

      //Verifica se o usuario digitou uma opcao invalida
      if (user_choice != 0 && user_choice != 1 && user_choice != 2)
      {
        printf("\n\rOpcao invalida!!!\n\r Pressione Enter.\n\n\r\r");
        system_pause();
      }

      //Se o usuario escolher a opcao de sair
      if (user_choice == 2)
      {
        printf("\n\r\n\rEscalonador finalizado...\n\r");
        return 0;
        exit;
      }

      //Verifica se a opcao escolhida foi criar uma nova tarefa, se sim, retorna 1
      if (user_choice == 0)
        return 1;
      //Verifica se a opcao escolhida foi de processar uma tarefa da fila, se sim, retorna 0
      else if (user_choice == 1)
        return 0;
    }
  }
}

/**
 * Função que apresenta o MENU para preencher uma tarefa e retorná-la 
*/
Task create_task_cli()
{
  system("clear");
  char *p_name = (char *)malloc(50 * sizeof(char)); //50 caracateres
  int total_instructions = 0;

  printf("\n\r==================================CREATE TASK==================================\n\r");
  printf("\n\r- Informe o nome: ");
  scanf("%s", p_name); //Não funciona no Linux
  // fflush(stdin);
  // p_name = read_input_string(stdin, 10);
  // fflush(stdin);

  printf("\n\r- Informe o total de instrucoes da tarefa: ");
  scanf("%d", &total_instructions);

  printf("\n\rTarefa criada com sucesso!\n\r");
  return task_construct(p_name, total_instructions);
}

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

  //Armanzenando em que ciclo a tarefa entrou na fila do escalonador para ser executada
  p_task->start_cycle = count_cycles;

  //Executa de 1 até 10 ciclos de processamento
  for (int i = 0; i < 10; i++)
  {
    count_cycles++;
    printf("Executando a tarefa: %s \n\r", p_task->name);
    p_task->counter++; //Incrementando o contador de ciclos

    //Verificando se a tarefa executou até o final p/ retornar 1
    if (p_task->counter == p_task->total_instructions)
    {
      p_task->end_cycle = count_cycles;
      system_pause();
      return 1;
    }
  }

  system_pause();
  //Se não executou até o final retorna 0
  return 0;
}

//Processando as tarefas da fila
int main()
{
  //Fila de tarefas a serem enviadas para o escalonador
  Queue *q = queue_construct();

  //Armazena o número de tarefas atual no escalonador
  int qtd_tasks = 0;

  //Checa se o retorno foi diferente de 1, que representa a opcao de criar uma tarefa e se for, finaliza o programa
  if (manager_menus(FIRST_MENU) != 1)
  {
    return 0;
    exit;
  }

  //Apresenta a interface de menu para criar uma nova task ao usuario
  insert_end(q, create_task_cli());

  //Imprimindo a fila cheia
  print_queue(q);
  system_pause();

  //Armazena o número de tarefas terminadas
  int count_finished = 0;

  while (q->qtd_nos > 0)
  {
    //Passando a tarefa a ser processada e verificando se ela foi executada até o final
    if (process(&(q->first_no->info)) == 1)
    {
      remove_begin(q);
      count_finished++;
    }
    //Se a tarefa não foi processada até o final ela volta para o final da fila
    else
    {
      //Tirando a tarefa do inicio a passando para o final
      from_begin_to_end(q);
    }

    //Apos processar uma tarefa, apresenta-se o menu perguntando se o usuario deseja criar outra tarefa ou continuar a processar outra tarefa
    //!OBS: Se o usuario escolher criar outra tarefa novamente o while permanecera com user_choice = 1, logo, apresentara o menu novamente
    int user_choice = 1;
    while (user_choice == 1)
    {
      user_choice = manager_menus(MAIN_MENU);
      if (user_choice == 1)
      {
        //Apresenta a interface de menu para criar uma nova task ao usuario
        insert_end(q, create_task_cli());
      }
    }
  }

  // //Imprimindo a fila vazia
  // imprimir_fila(&queue);

  //Imprimindo o número total de ciclos utilizados para processar todas as tarefas da fila
  printf("\n\r**********************************************************************************");
  printf("\r\nNumero total de cilos utilizado: %d\n\r", count_cycles);
  return 0;
}
