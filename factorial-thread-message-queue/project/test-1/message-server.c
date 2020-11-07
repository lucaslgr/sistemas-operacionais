// C Program for Message Queue (Writer Process) to calculate a factorial
#include <stdio.h>
#include <stdlib.h> //To use atoi, atof, etc
#include <string.h> //To use strcpy

#include <sys/ipc.h> //To use ftok and key_t
#include <sys/msg.h> //To send and receive msg

#include "message-reader.c"

//Constante que define o número de processos
#define NUM_READ_PROCESS 2

//Identifica o fatorial e separa em 3 para ser calculado por cada thread
void split_fatorial(int num, int num_process, int *fac_offset)
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

//Copia os elementos de um array para outro
void array_copy(int *empty_array, int *full_array)
{
  int array_size = (int)(sizeof(full_array) / sizeof(int));
  for (int i = 0; i < array_size; i++)
    empty_array[i] = full_array[i];
}

//Envia as mensagens para a respectiva fila de mensagem
void send_message(int id_queue_messages, message message_to_send)
{
  key_t key; //Armazena a chave da fila de mensagem
  int msgid; //Armazena o identificador da mensagem

  // ftok to generate unique key
  // ftok gera uma chave única para a fila de mensagens
  key = ftok("progfile", 65);

  // msgget creates a message queue and returns identifier
  // msgget cria uma fila de mensagens e retorna o identificador
  msgid = msgget(key, 0666 | IPC_CREAT);

  // msgsnd to send message
  // msgsnd para enviar uma mensagem
  msgsnd(msgid, &message_to_send, sizeof(message), 0);

  // display the message
  // mostra a mensagem enviada
  printf("Nome da mensagem enviada : %s | limites %d, %d\n",
         message_to_send.mesg_text,
         message_to_send.fac_parts[0],
         message_to_send.fac_parts[1]);
}

int main(int agrc, char **agrv)
{
  int num = 0;
  printf("Informe o numero que deseja calcular o fatorial: ");
  scanf("%d", &num);

  //!BUILDING THE DATA OF STRUCT MESSAGES TO SEND TO EACH PROCESS
  //Defining the messages structs data
  message msg_queue[NUM_READ_PROCESS];
  for (int i = 0; i < NUM_READ_PROCESS; i++)
  {
    msg_queue[i].mesg_type = (i + 1);
    sprintf(msg_queue[i].mesg_text, "%dª Message", (i + 1));
  }

  int fac_parts_aux[NUM_READ_PROCESS];
  // splitting the factorial in parts respective to number of process
  split_fatorial(num, NUM_READ_PROCESS, fac_parts_aux);

  for (int i = 0; i < NUM_READ_PROCESS; i++)
    array_copy(msg_queue[i].fac_parts, fac_parts_aux);

  //!SENDING EACH MESSAGE TO RESPECTIVE PROCESS
  for (int i = 0; i < NUM_READ_PROCESS; i++)
    send_message(65, msg_queue[i]);
}