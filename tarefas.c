#include <stdio.h>
#include <stdlib.h>
#include<pthread.h>

struct dados_t {
  int id;
  int cont;
  char *msg;
};

void* tarefa(void *dados)
{
	struct dados_t *dd = (struct dados_t *)dados;

	printf("Tarefa %d, contador %d disse: %s\n", 
			dd->id, dd->cont,dd->msg);
	dd->cont += 5;
}

int main()
{
    // declara os parametros das tarefas
    struct dados_t dt1, dt2, dt3;

    // declara as tarefas
    pthread_t t1, t2, t3;

    // configura os parâmetros de cada tarefa
    dt1.id = 1;
    dt1.cont = 0;
    dt1.msg = "Ola!";

    dt2.id = 2;
    dt2.cont = 7;
    dt2.msg = "Hey!";

    dt3.id = 2;
    dt3.cont = 3;
    dt3.msg = "Oi!";

    // cria as tarefas
    pthread_create(&t1, NULL, tarefa, (void *)&dt1); 
    pthread_create(&t2, NULL, tarefa, (void *)&dt2); 
    pthread_create(&t3, NULL, tarefa, (void *)&dt3); 
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    // mostra os valores dos dados modificados
    printf("Tarefa %d, contador %d\n", dt1.id, dt1.cont);
    printf("Tarefa %d, contador %d\n", dt2.id, dt2.cont);
    printf("Tarefa %d, contador %d\n", dt3.id, dt3.cont);

    return 0;
}
