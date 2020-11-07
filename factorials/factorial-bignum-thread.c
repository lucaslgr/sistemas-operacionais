/**! INSTRUÇÕES
*! Para compilar:
*!  - gcc -o test fatorial-bignum-thread.c -lpthread
*! Para executar:
*!  - ./test <VALUE TO CALCULATED THE factorial> <NUMBER OF THREADS YOU WISH TO USE>
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>      //Para criar threads
#include "./lib/bignum.c" //Lib não nativa para utilizar o tipo bignum
#include <time.h>         //Utilizada para tempo

//* Estrutura que armazena os fornece e armazena os dados de cada thread
struct data_task
{
    int *p_offsets;
    int indicator_part;
    bignum partial_result;
    // int partial_result;
};

//*Tarefa executada por cada thread
void *task_thread(void *task_dt)
{
    struct data_task *p_data_task = (struct data_task *)task_dt;

    int_to_bignum(1, &p_data_task->partial_result);

    //Calculando os limites superiores e inferiores
    int upper_limit = p_data_task->p_offsets[p_data_task->indicator_part];

    int inferior_limit = 1;
    if (p_data_task->indicator_part > 0)
    {
        inferior_limit = p_data_task->p_offsets[p_data_task->indicator_part - 1];
    }

    for (int i = upper_limit; i > inferior_limit; i--)
    {
        bignum iterator;
        int_to_bignum(i, &iterator);

        bignum product;
        multiply_bignum(&p_data_task->partial_result, &iterator, &product);

        p_data_task->partial_result = product;
    }

    printf(
        "\r\n====================================Thread %d INFO====================================\n\rIndicator %d | Upper_Limit %d | Inferior_Limit %d \n\rResultado parcial do fatorial pela thread %d : ",
        p_data_task->indicator_part + 1, p_data_task->indicator_part + 1, upper_limit, inferior_limit, p_data_task->indicator_part);
    print_bignum(&p_data_task->partial_result);
    printf("\n\n\r\r");
}

//Identifica o fatorial e separa em 3 para ser calculado por cada thread
void split_fatorial(int num, int num_threads, int *fac_offset)
{
    int rest;
    rest = num % num_threads;

    if (rest == 0)
    {
        int offset = num / num_threads;

        for (int i = 1; i <= num_threads; i++)
        {
            fac_offset[i - 1] = offset * i;
        }
    }
    else
    {
        int offset = (int)num / num_threads;

        for (int i = 1; i <= num_threads; i++)
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

//Verifica se o número é 0, 1 ou 2. Se for, já retorna o resultado
void solve_if_small_number(int *num)
{

    if (*num == 0 || *num == 1)
    {
        printf("Resposta: %d", 1);
        exit;
        return;
    }
    else if (*num == 2)
    {
        printf("Resposta: %d", 2);
        exit;
        return;
    }
}

//Multiplica todos os resultados parciais do fatorial
void join_partial_results(struct data_task *p_data_tasks, bignum *final_result, int num_threads)
{
    bignum partial_result = (p_data_tasks + 0)->partial_result;
    for (int i = 0; i < num_threads - 1; i++)
    {
        multiply_bignum(&partial_result, &(p_data_tasks + i + 1)->partial_result, final_result);
        partial_result = *final_result;
    }
}

//!OBS:
//!Primeiro argumento do argv é o numero a se calcular o fatorial
//!Segundo argumento do argv é o numero de threads que serão utilizadas para o calculo
int main(int argc, char *argv[])
{
    //===================================<CÁLCULO DO FATORIAL COM BIGNUM USANDO THREADS>===================================
    //pritnf("Informe o numero que deseja calcular o fatorial: ");
    //scanf("%d", &num);

    //Variaveis para armazenar os clocks do processador para medir o tempo no final
    clock_t start, end;

    //Variavel que armazena o numero de threads que o usuário deseja utilizar para calcular o fatorial
    int num_threads = atoi(argv[2]);

    //Variaveis que armanzenarão o numero que o usuario deseja calcular o fatorial
    bignum bignum_n;
    int num;
    //Convertendo de char para inteiro, agrv[1] é referente ao número para se calcular o fatorial
    num = atoi(argv[1]);
    printf("Calculando fatorial de: %d \n\r", num);

    //Inicializando a contagem de clocks
    start = clock();

    //Resolvando o fatorial caso o num desejado seja 0,1 ou 2
    solve_if_small_number(&num);

    //Verificando se o número que o usuário deseja calcular o fatorial é maior que o número de threads que ele deseja usar, caso seja o número de threads será setado no mesmo valor do número que se deseja calcular o fatorial
    if (num_threads > num)
        num_threads = num;

    //Vetor que armazenará cada número que limitara até onde cada thread deve calcular
    int *fac_offset = (int *)malloc(sizeof(int) * num_threads);

    //Divindo o número em partes iguais para enviar para cada thread
    split_fatorial(num, num_threads, fac_offset);

    //Declarando o vetor de structs das estruturas que armazenarão os dados de resolução de cada thread
    struct data_task *p_datatasks = (struct data_task *)malloc(sizeof(struct data_task) * num_threads);

    //Setando os dados de cada estrutura que cada thread vai resolver
    // struct data_task dt1, dt2, dt3;

    for (int i = 0; i < num_threads; i++)
    {
        (p_datatasks + i)->indicator_part = i;
        (p_datatasks + i)->p_offsets = fac_offset;
    }

    pthread_t *p_threads = (pthread_t *)malloc(sizeof(pthread_t) * num_threads);

    // cria as tarefas
    for (int i = 0; i < num_threads; i++)
    {
        pthread_create((p_threads + i), NULL, task_thread, (void *)(p_datatasks + i));
    }

    //Coleta o dado das tarefas
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(*(p_threads + i), NULL);
    }

    bignum final_result;
    join_partial_results(p_datatasks, &final_result, num_threads);

    //Finalizando a contagem de clocks
    end = clock();

    //Calculando o tempo decorrido
    double tmp = (double)(end - start);
    tmp = tmp / (double)CLOCKS_PER_SEC;
    printf("\nTempo decorrido: %f\n", tmp);

    //Printando o resultado final
    printf("Resultado Final: ");
    print_bignum(&final_result);

    return 0;
}