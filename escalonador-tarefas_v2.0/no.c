#pragma once
#include "task.c"  //Importando uma estrutura de uma tarefa
#include <stdio.h> //Para poder usar NULL, para usar printf e scanf, para usar o getchar()

/**
 * Estrutura de um No
*/
struct no
{
  Task info;
  struct no *next;
  struct no *prev;
} typedef No;

/**
 * Constroi um No que armazena uma task e o retorna 
*/
No no_construct(Task t)
{
  No n;
  n.info = t;
  n.next = NULL;
  n.prev = NULL;
  return n;
}

/**
 * Seta o endereço do proximo no 
*/
void set_next(No *current_no, No *next_no)
{
  current_no->next = next_no;
}

/**
 * Seta o endereço do no anterior 
*/
void set_prev(No *current_no, No *prev_no)
{
  current_no->prev = prev_no;
}

/**
 * Retorna a task que o No armazena em info 
*/
Task get_info(No *current_no)
{
  return current_no->info;
}

/**
 * Retorna o endereço para o proximo No que o current_no aponta 
*/
No *get_next(No *current_no)
{
  return current_no->next;
}