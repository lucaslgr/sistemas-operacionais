#include <stdio.h>  //para usar printf e scanf
#include <stdlib.h> //para usar o malloc

#include "tarefa.c" //importa todas definicoes de uma tarefa

//!FILA CONTIGUA CIRCULAR
struct fila_circular
{
  //Armazena a capacidade maxima de elementos na fila
  int m;
  //Armazena a posicao do primeiro elemento da fila
  int c;
  //Armazena a primeira posicao livre apos o ultimo elemento da fila
  int f;
  //Armazena o numero de elementos que existem na fila
  int n;
  //Armazena o vetor com todas tarefas da fila
  struct tarefa *x;
};

//Construtor da fila circular
struct fila_circular fila_circular_construct(int max)
{
  struct fila_circular queue;
  queue.m = max;
  queue.c = 0;
  queue.f = 0;
  queue.n = 0;

  queue.x = (struct tarefa *)malloc(sizeof(struct tarefa) * max);

  return queue;
}

//Função que retorna o incremento correto para c ou f
int incremento(struct fila_circular *p_queue, int indice)
{
  //Verifica se chegou no último indice, se chegou, retorna o indice inicial -> 0
  if (indice == (p_queue->m - 1))
    return 0;
  //Incrementa 1 no indice atual se não chegou no último
  else
    return indice + 1;
}

//Função que insere um elemento no final da fila
void insere_final(struct fila_circular *p_queue, struct tarefa val)
{
  if (p_queue->n == p_queue->m)
    printf("A fila esta cheia!\n\r");
  else
  {
    p_queue->x[p_queue->f] = val;
    p_queue->f = incremento(p_queue, p_queue->f);
    p_queue->n += 1;
  }
}

//Função que remove do inicio da fila
void remove_inicio(struct fila_circular *p_queue)
{
  if (p_queue->n == 0)
    printf("Lista vazia!!!");
  else
  {
    p_queue->c = incremento(p_queue, p_queue->c);
    p_queue->n -= 1;
  }
}

//Retorna o tamanho da fila
int get_tam(struct fila_circular *p_queue)
{
  return p_queue->n;
}

//Imprime todos elementos da fila
void imprimir_fila(struct fila_circular *p_queue)
{
  printf("\n\r\t\t\t\t\t====================================FILA INFO====================================\n\r");
  printf("\t\t\t\t\tTamanho maximo 'm': %d |\n\r\t\t\t\t\tPosicao primeiro elemento 'c': %d |\n\r\t\t\t\t\tPosicao livre apos o ultimo elemento 'f': %d |\n\r\t\t\t\t\tNumero de elementos da fila 'n': %d |\n\r", p_queue->m, p_queue->c, p_queue->f, p_queue->n);

  if (p_queue->n == 0)
    printf("\nNao ha o que imprimir, a fila esta vazia!!!\n\r");
  else
  {
    printf("\n\r\t\t\t\t\t================================IMRPIMINDO A FILA================================\n\r\t\t\t\t\t");

    for (int i = 0; i < p_queue->n; i++)
    {
      printf("%s ", p_queue->x[p_queue->c + i].nome);
    }
    printf("\n\r");
  }
}