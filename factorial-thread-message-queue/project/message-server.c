// C Program for Message Queue (Writer Process) to calculate a factorial
#include <stdio.h>
#include <stdlib.h> //To use atoi, atof, etc
#include <string.h> //To use strcpy

#include <sys/ipc.h> //To use ftok and key_t
#include <sys/msg.h> //To send and receive msg

//Constante que define o número de processos
#ifndef NUM_READ_PROCESS
#define NUM_READ_PROCESS 2
#endif

//Constante para criar a key identificadora da fila de mensagens enviada para os readers 1 e 2
//Constante que define o nome identificador do arquivo da fila de mensagens enviada para os readers 1 e 2
#if !defined ID_QUEUE_MESSAGES_TO_READERS || !defined ID_QUEUE_MESSAGES_TO_READERS
#define ID_QUEUE_MESSAGES_TO_READERS 60
#define NAME_FILE_QUEUE_MESSAGES_TO_READERS "prog60"
#endif

//Constante para criar a key identificadora da fila de mensagens recebidas dos readers 1 e 2
//Constante que define o nome identificador do arquivo da fila de mensagens recebidas dos readers 1 e 2
#if !defined ID_QUEUE_MESSAGES_BACKTO_READERS || !defined ID_QUEUE_MESSAGES_BACKTO_READERS
#define ID_QUEUE_MESSAGES_BACKTO_READERS 61
#define NAME_FILE_QUEUE_MESSAGES_BACKTO_READERS "prog61"
#endif

//Estrutura para usar a fila de mensagens
struct mesg_buffer
{
  long mesg_type;
  char mesg_text[100];
  int fac_parts[NUM_READ_PROCESS];
  int partial_result;
} typedef message;

//Identifica o fatorial e separa em 3 para ser calculado por cada thread
void split_factorial(int num, int num_process, int *fac_offset);

//Envia as mensagens para a respectiva fila de mensagem
void send_message(int id_queue_messages, message message_to_send);

//Recebe as mensagem para a respectiva fila de mensagem e do respectivo tipo, calcula o fatorial parcial respectivo as informacoes passadas na mensagem e retorna o resultado
int receive_message(int id_queue_messages, message msg, long msg_type, int flag_destroy_queue);

//Copia os elementos de um array para outro
void array_copy(int *empty_array, int *full_array);

//Constroi uma struct message com atributos resetados
message message_construct();

int main(int agrc, char **agrv)
{
  printf("\n====================================SERVIDOR INICIALIZADO====================================\n");
  int num = 0;
  printf("Informe o numero que deseja calcular o fatorial: ");
  scanf("%d", &num);

  printf("\n====================================CONSTRUINDO MENSAGENS====================================\n");
  //!BUILDING THE DATA OF STRUCT MESSAGES TO SEND TO EACH PROCESS
  //Defining the messages structs data
  message msg_queue[NUM_READ_PROCESS];
  for (int i = 0; i < NUM_READ_PROCESS; i++)
  {
    printf("...\n");
    msg_queue[i].mesg_type = (i + 1);
    sprintf(msg_queue[i].mesg_text, "%dª Message", (i + 1));
  }

  int fac_parts_aux[NUM_READ_PROCESS];
  // splitting the factorial in parts respective to number of process
  split_factorial(num, NUM_READ_PROCESS, fac_parts_aux);

  for (int i = 0; i < NUM_READ_PROCESS; i++)
    array_copy(msg_queue[i].fac_parts, fac_parts_aux);

  printf("\n==========================================ENVIANDO==========================================\n");
  //!SENDING EACH MESSAGE TO RESPECTIVE PROCESS
  for (int i = 0; i < NUM_READ_PROCESS; i++)
    send_message(ID_QUEUE_MESSAGES_TO_READERS, msg_queue[i]);

  printf("\n==========================================RECEBENDO==========================================\n");
  //!RECEINVING MESSAGES
  message msg_received[NUM_READ_PROCESS];
  int partial_results[NUM_READ_PROCESS];
  //Resultado do reader1 e reader2 estão nas mensagens de tipo 3 e 4 respectivamente
  for (int i = 0; i < NUM_READ_PROCESS; i++)
  {
    //Verifica se chegou na leitura da ultima mensagem, se chegou, envia flag para destruir a fila de mensagens
    if (i == (NUM_READ_PROCESS - 1))
      partial_results[i] = receive_message(ID_QUEUE_MESSAGES_BACKTO_READERS, msg_received[i], (NUM_READ_PROCESS + i + 1), 1);
    else
      partial_results[i] = receive_message(ID_QUEUE_MESSAGES_BACKTO_READERS, msg_received[i], (NUM_READ_PROCESS + i + 1), 0);
  }

  printf("\n===========================================JUNTANDO===========================================\n");
  int final_result = 1;
  for (int i = 0; i < NUM_READ_PROCESS; i++)
    final_result *= partial_results[i];

  printf("Resultado final: %d\n", final_result);
}

void split_factorial(int num, int num_process, int *fac_offset)
{
  int rest;
  rest = num % num_process;

  if (rest == 0)
  {
    int offset = num / num_process;

    for (int i = 1; i <= num_process; i++)
    {
      fac_offset[i - 1] = offset * i;
    }
  }
  else
  {
    int offset = (int)num / num_process;

    for (int i = 1; i <= num_process; i++)
    {
      if (i == 1)
        fac_offset[i - 1] = (offset * i) + rest; //Adicionando o resto para completar o número quebrado
      else
      {
        fac_offset[i - 1] = fac_offset[i - 1 - 1] + offset;
      }
    }
  }
}

void send_message(int id_queue_messages, message message_to_send)
{
  key_t key; //Armazena a chave da fila de mensagem
  int msgid; //Armazena o identificador da mensagem

  // ftok to generate unique key
  // ftok gera uma chave única para a fila de mensagens
  key = ftok(NAME_FILE_QUEUE_MESSAGES_TO_READERS, id_queue_messages);

  // msgget creates a message queue and returns identifier
  // msgget cria uma fila de mensagens e retorna o identificador
  msgid = msgget(key, 0666 | IPC_CREAT);

  // msgsnd to send message
  // msgsnd para enviar uma mensagem
  msgsnd(msgid, &message_to_send, sizeof(message), 0);

  // display the message
  // mostra a mensagem enviada
  printf("Nome da mensagem enviada : %s | Tipo: %lu | limites %d, %d\n",
         message_to_send.mesg_text,
         message_to_send.mesg_type,
         message_to_send.fac_parts[0],
         message_to_send.fac_parts[1]);
}

int receive_message(int id_queue_messages, message msg, long msg_type, int flag_destroy_queue)
{
  key_t key; //Armazena a chave da fila de mensagem
  int msgid; //Armazena o identificador da mensagem

  // ftok to generate unique key
  // ftok gera uma chave única para a fila de mensagens
  key = ftok(NAME_FILE_QUEUE_MESSAGES_BACKTO_READERS, id_queue_messages);

  // msgget creates a message queue and returns identifier
  // msgget cria uma fila de mensagens e retorna o identificador
  msgid = msgget(key, 0666 | IPC_CREAT);

  // calculando o numero do reader que esta esperando receber o resultado pelo numero do tipo da mensagem
  int num_reader = msg_type - NUM_READ_PROCESS;

  //Verificando se a mensagem com o resultado parcial foi recebida
  printf("Verificando se a mensagem com msgid: %d e o resultado parcial calculado pelo reader %d foi recebida...\n\r", msgid, num_reader);

  // msgrcv to receive message
  // msgrcv para receber mensagem
  // Verifica se a mensagem foi recebida até que seja recebida
  msgrcv(msgid, &msg, sizeof(message), msg_type, 0);

  // display the message
  // mostra a mensagem recebida
  printf("Nome da mensagem recebida : %s| Tipo: %lu | Resultado parcial: %d\n\r",
         msg.mesg_text,
         msg.mesg_type,
         msg.partial_result);

  printf("\n\r*****************************\n\r");

  // to destroy the message queue
  // destroi a fila de mensagens
  if (flag_destroy_queue == 1)
  {
    printf("Fila de mensagens: %s destruida... \n\r", NAME_FILE_QUEUE_MESSAGES_BACKTO_READERS);
    msgctl(msgid, IPC_RMID, NULL);
  }

  return msg.partial_result;
}

void array_copy(int *empty_array, int *full_array)
{
  int array_size = (int)(sizeof(full_array) / sizeof(int));
  for (int i = 0; i < array_size; i++)
    empty_array[i] = full_array[i];
}

message message_construct()
{
  message m;
  m.mesg_type = -1;
  for (size_t i = 0; i < NUM_READ_PROCESS; i++)
    m.fac_parts[i] = 0;
  m.partial_result = 0;

  return m;
}