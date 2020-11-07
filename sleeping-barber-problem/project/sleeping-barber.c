#ifndef MAX_CUSTOMERS // CONSTANTE que define o numero MÁXIMO de clientes a irem na barbearia
#define MAX_CUSTOMERS 20
#endif

#include <stdio.h>  //printf scanf
#include <unistd.h> // sleep
#include <stdlib.h> //exit, rand
#include <pthread.h>
#include <semaphore.h>

//!Declarando a assinatura dos dois métodos que serão executados pelas threads
void *barber(void *thread_param);
void *customer(void *thread_param);

//!Declarando os semaforos
//Semáforo que conta os clientes à espera de atendimento exceto o que já está na cadeira do barbeiro
sem_t semaphore_customers;

//Semáforo que conta os barbeiros que estão à espera de clientes
sem_t semaphore_barbers;

//Semáforo binário(ou seja, um mutex) que é utilizado para exclusão mútua ao mexer na variavel n_customers_wating que representa os clientes esperando nas cadeiras
sem_t mutex_customers_waiting;

//Contador de clientes à espera
int n_customers_wating;

//Armazena o numero de cadeiras setadas pelo usuario
int n_avaible_chairs;

//Armazena o maximo tempo de espera do cliente entre o proximo corte ou a proxima tentativa de corte
int customer_execution_delay = 0;

int main(int agrc, char **agrv)
{
  pthread_t barber_thread;
  pthread_t customers_threads[MAX_CUSTOMERS];

  //!1º - Pegando os argumentos, barber_execution_delay, num_customers, n_avaible_chairs, client_execution_delay
  int main_thread_delay = 0,
      num_customers = 0;

  printf("Informe os seguintes argumentos no respectivo formato:[NUM_CUSTOMERS] [NUM_AVAIBLE_CHAIRS] [DELAY_CUSTOMERS] [DELAY_MAIN_THREAD] \n\r");
  scanf("%d %d %d %d", &num_customers, &n_avaible_chairs, &customer_execution_delay, &main_thread_delay);

  if (num_customers == 0 || n_avaible_chairs == 0 || customer_execution_delay == 0 || main_thread_delay == 0)
  {
    printf("Informacoes nao setadas corretamente\n\r");
    exit(0);
  }

  //!2º - Inicializando os semaforos
  sem_init(&mutex_customers_waiting, 0, 1);
  sem_init(&semaphore_customers, 0, 0);
  sem_init(&semaphore_barbers, 0, 0);

  //!3º - Cria thread do barbeiro executar o método barber
  pthread_create(&barber_thread, NULL, barber, NULL);
  printf("Thread do barbeiro criada e executando o metodo barber, id: %lu \n\r", barber_thread);

  //!4ª - Criando as threads dos clientes
  for (unsigned int i = 0; i < num_customers; i++)
  {
    pthread_create(&customers_threads[i], NULL, customer, NULL);
    printf("Thread do cliente criada e executando o metodo customer, id: %lu \n\r", customers_threads[i]);
  }

  //!5ª - Thread principal dormindo
  printf("Thread principal dormindo por %d segundos\n\r", main_thread_delay);
  sleep(main_thread_delay);

  printf("Thread principal finalizada \n\r");
  exit(0);
}

/*//!Metodo Barber
 * - 1º Espera por um cliente 
 * - 1.1º Vai dormir e permanece até que o primeiro cliente apareça
 * - 2º Bloqueia o semaforo mutex_customers_waiting que inicialmente esta em zero
 * - 2.1º Incrementa o numero de cadeiras disponiveis
 * - 2.2º Decrementa o numero de clientes esperando
 * - 3º Sinaliza para o cliente que o barbeiro esta pronto para cortar o seu cabelo
 * - 4º Libera o semaforo mutex_customers_waiting
 * - 5º Gera um numero aleatorio que representa o delay_work_time de 1 a 4 segundos para cada corte de cabelo
 * - 6º Corta o cabelo durante delay_work_time de segundos : sleep(delay_work_time)
*/
void *barber(void *thread_param)
{
  int delay_work_time = 0;

  for (;;)
  {
    //1º e //1.1º
    sem_wait(&semaphore_customers);

    //2º
    sem_wait(&mutex_customers_waiting);
    n_avaible_chairs += 1;   //2.1º
    n_customers_wating -= 1; //2.2º
    printf("Barbeiro pegou um cliente | Cadeiras Disponiveis: %d\n\r", n_avaible_chairs);

    //3º
    sem_post(&semaphore_barbers);

    //4º
    sem_post(&mutex_customers_waiting);

    //5º
    delay_work_time = (rand() % 4) + 1;

    //6º
    printf("Barbeiro cortando o cabelo por %d segundos\n\r", delay_work_time);
    sleep(delay_work_time);
  }
}

/*//!Metodo Customer
 * - 1º Obtem o mutex mutex_customers_waiting para entrar na regiao critica
 * - 2º O cliente verfica se existem cadeiras disponíveis, [clientes  à espera < cadeiras disponiveis]?
 * - 2.1º Se SIM
 * - 2.1.1º incrementa a variavel n_customers_wating e decrementa variavel n_avaible_chairs de cadeiras disponiveis 
 * - 2.1.2º faz um up no semaforo semaphore_customers que acorda o barbeiro
 * - 2.1.3º libera o mutex mutex_customers_waiting para a contagem de clientes esperando e cadeiras disponiveis
 * - 2.1.4º espera pelo barbeiro desocupar
 * - 2.1.5º cliente entra para cortar o cabelo
 * - 2.2º Se NÃO
 * - 2.2.1º libera o mutex mutex_customers_waiting e sai sem cortar cabelo
 * - 3º Gera um numero aleatorio de acordo com o maximo setado pelo usuario para um tempo de espera entre o proximo corte de cabelo ou a proxima tentativa.
*/
void *customer(void *thread_param)
{
  int delay_wait_time = 0;

  for (;;)
  {
    //1º
    sem_wait(&mutex_customers_waiting);

    //2º
    if (n_customers_wating < n_avaible_chairs)
    {
      //2.1º
      //2.1.1º
      n_customers_wating += 1;
      n_avaible_chairs -= 1;

      printf("Cliente Thread %u sentou para esperar | Cadeiras Disponiveis: %d\n\r", (unsigned int)pthread_self(), n_avaible_chairs);

      //2.1.2º
      sem_post(&semaphore_customers);

      //2.1.3º
      sem_post(&mutex_customers_waiting);

      //2.1.4
      sem_wait(&semaphore_barbers);

      //2.1.5
      printf("Cliente Thread %u esta cortando o cabelo\n\r", (unsigned int)pthread_self());
    }
    else
    {
      //2.2º
      printf("Cliente Thread %u esta saindo sem cortar o cabelo pois nao havia cadeiras disponiveis\n\r", (unsigned int)pthread_self());
      //2.2.1º
      sem_post(&mutex_customers_waiting);
    }

    //3º
    delay_wait_time = (rand() % customer_execution_delay) + 1;
    printf("Cliente Thread %u esperando %d segundos antes da tentiva do proximo corte\n\r", (unsigned int)pthread_self(), delay_wait_time);
    sleep(delay_wait_time);
  }
}