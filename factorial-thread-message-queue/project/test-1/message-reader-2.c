// C Program for Message Queue (Writer Process) to calculate a factorial
#include <stdio.h>
#include <stdlib.h> //To use atoi, atof, etc
#include <string.h> //To use strcpy

#include <sys/ipc.h> //To use ftok and key_t
#include <sys/msg.h> //To send and receive msg

//Constante que define o número de processos
#define NUM_READ_PROCESS 2

//Estrutura para usar a fila de mensagens
struct mesg_buffer
{
  long mesg_type;
  char mesg_text[100];
  int fac_parts[NUM_READ_PROCESS];
} typedef message;

void receive_message(int id_queue_messages, message msg, long msg_type)
{
  key_t key; //Armazena a chave da fila de mensagem
  int msgid; //Armazena o identificador da mensagem

  // ftok to generate unique key
  // ftok gera uma chave única para a fila de mensagens
  key = ftok("progfile", id_queue_messages);

  // msgget creates a message queue and returns identifier
  // msgget cria uma fila de mensagens e retorna o identificador
  msgid = msgget(key, 0666 | IPC_CREAT);

  // msgrcv to receive message
  // msgrcv para receber mensagem
  msgrcv(msgid, &msg, sizeof(message), msg_type, 0);

  // display the message
  // mostra a mensagem recebida
  printf("Nome da mensagem recebida : %s| limites %d, %d\n",
         msg.mesg_text,
         msg.fac_parts[0],
         msg.fac_parts[1]);

  // to destroy the message queue
  // destroi a fila de mensagens
  // msgctl(msgid, IPC_RMID, NULL);
}

int main(int agrc, char **agrv)
{
  message m2;
  receive_message(65, m2, 2);
}