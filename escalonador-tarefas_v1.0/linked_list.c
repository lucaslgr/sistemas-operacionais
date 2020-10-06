//!BIBLIOTECAS NATIVAS
#include <stdlib.h> //para usar o malloc
#include "no.c"     //importa todas definicoes de uma tarefa

struct linked_list
{
  //Armazena o endereço para o primeiro No da fila
  No *first_no;

  //Armazena o endereço para o ultimo No da fila
  No *last_no;

  //Armazena a quantidade de No da fila
  int qtd_nos;
} typedef Queue;

//Construtor da fila baseada em uma estrutura de lista encadeada
Queue queue_construct()
{
  Queue q;
  q.first_no = NULL;
  q.last_no = NULL;
  q.qtd_nos = 0;
  return q;
}

/**
 *Insere no final da fila 
*/
void insert_end(Queue *q, Task t)
{
  //Se a lista esta vazia
  if (q->first_no == NULL)
  {
    No *p_new_no = (No *)malloc(sizeof(No));
    *p_new_no = no_construct(t);

    //Setando o primeiro e ultimo apontando para o ÚNICO nó criado
    q->first_no = p_new_no;
    q->last_no = p_new_no;
    q->qtd_nos++;
    return;
  }
  //Se a lista não esta vazia
  else
  {
    No *p_new_no = (No *)malloc(sizeof(No));
    *p_new_no = no_construct(t);

    No *p_current_last_no = q->last_no;
    q->last_no = p_new_no;
    q->last_no->prev = p_current_last_no;
    p_current_last_no->next = q->last_no;
    q->qtd_nos++;
    return;
  }
}

/**
 * Remove do inicio
*/
void remove_begin(Queue *q)
{
  if (q->qtd_nos == 0)
  {
    printf("\n\rA fila esta vazia!!!\n\r");
  }
  else
  {
    No *p_current_second = q->first_no->next;
    free(q->first_no);

    q->first_no = p_current_second;
    q->first_no->prev = NULL;
    q->qtd_nos--;
  }
}

/**
 * Do inicio para o fim da fila 
*/
void from_begin_to_end(Queue *q)
{
  No *p_current_first_no = q->first_no;
  No *p_current_last_no = q->last_no;

  q->first_no = p_current_first_no->next;
  q->first_no->prev = NULL;

  q->last_no = p_current_first_no;
  p_current_last_no->next = q->last_no;
  q->last_no->prev = p_current_last_no;
  q->last_no->next = NULL;
}

/**
 * Retorna o número de elementos na fila
*/
int get_size_queue(Queue *q)
{
  return q->qtd_nos;
}

/**
 * Imprime a fila
*/
void print_queue(Queue *q)
{
  No *p_current_no = q->first_no;

  printf("\n\r===================================IMPRIMINDO A LISTA===================================\n\r");
  printf("Nomes: ");
  while (p_current_no != NULL)
  {
    printf("%s ", p_current_no->info.name);
    p_current_no = p_current_no->next;
  }
  printf("\n\r");
}