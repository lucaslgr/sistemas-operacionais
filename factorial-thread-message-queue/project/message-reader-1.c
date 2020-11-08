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

//Recebe as mensagem para a respectiva fila de mensagem e do respectivo tipo, calcula o fatorial parcial respectivo as informacoes passadas na mensagem e retorna o resultado
int receive_message(int id_queue_messages, message msg, long msg_type);

//Envia as mensagens para a respectiva fila de mensagem
void send_message(int id_queue_messages, message message_to_send);

//Calcula uma parte do fatorial respectivo ao tipo passado que representa o indice da parte
int calc_partial_factorial(int factorial_parts[NUM_READ_PROCESS], long msg_type);

//Constroi uma struct message com atributos resetados
message message_construct();

int main(int agrc, char **agrv)
{
  message m1 = message_construct();
  message m1_result = message_construct();

  m1_result.mesg_type = 3;
  m1_result.partial_result = receive_message(ID_QUEUE_MESSAGES_TO_READERS, m1, 1);
  strcpy(m1_result.mesg_text, "1º Result Message");
  send_message(ID_QUEUE_MESSAGES_BACKTO_READERS, m1_result);
}

int receive_message(int id_queue_messages, message msg, long msg_type)
{
  printf("\n\r========================================RECEBENDO READER1========================================\n\r");
  key_t key; //Armazena a chave da fila de mensagem
  int msgid; //Armazena o identificador da mensagem

  // ftok to generate unique key
  // ftok gera uma chave única para a fila de mensagens
  key = ftok(NAME_FILE_QUEUE_MESSAGES_BACKTO_READERS, id_queue_messages);

  // msgget creates a message queue and returns identifier
  // msgget cria uma fila de mensagens e retorna o identificador
  msgid = msgget(key, 0666 | IPC_CREAT);

  // msgrcv to receive message
  // msgrcv para receber mensagem
  // Verifica se a mensagem foi recebida até que seja recebida
  while (msgrcv(msgid, &msg, sizeof(message), msg_type, 0) == -1)
  {
  }

  // display the message
  // mostra a mensagem recebida
  printf("Nome da mensagem recebida : %s| Tipo: %lu | limites %d, %d\n",
         msg.mesg_text,
         msg.mesg_type,
         msg.fac_parts[0],
         msg.fac_parts[1]);

  //calculando o fatorial parcial
  printf("\n\r==================================CALCULANDO O FATORIAL PARCIAL 1===================================\n\r");
  return calc_partial_factorial(msg.fac_parts, msg.mesg_type);
}

void send_message(int id_queue_messages, message message_to_send)
{
  key_t key; //Armazena a chave da fila de mensagem
  int msgid; //Armazena o identificador da mensagem

  // ftok to generate unique key
  // ftok gera uma chave única para a fila de mensagens
  key = ftok(NAME_FILE_QUEUE_MESSAGES_BACKTO_READERS, id_queue_messages);

  // msgget creates a message queue and returns identifier
  // msgget cria uma fila de mensagens e retorna o identificador
  msgid = msgget(key, 0666 | IPC_CREAT);

  // msgsnd to send message
  // msgsnd para enviar uma mensagem
  msgsnd(msgid, &message_to_send, sizeof(message), 0);

  // display the message
  // mostra a mensagem enviada
  printf("Nome da mensagem enviada : %s | Tipo %lu | Resultado Parcial: %d\n",
         message_to_send.mesg_text,
         message_to_send.mesg_type,
         message_to_send.partial_result);
}

int calc_partial_factorial(int factorial_parts[NUM_READ_PROCESS], long msg_type)
{

  int inferior_limit = (msg_type == 1) ? 1 : factorial_parts[msg_type - 2];
  int upper_limit = factorial_parts[msg_type - 1];
  int partial_result = 1;
  // printf("Limite inferior %d e limite superior %d\n", inferior_limit, upper_limit);

  for (int i = upper_limit; i > inferior_limit; i--)
    partial_result *= i;

  return partial_result;
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