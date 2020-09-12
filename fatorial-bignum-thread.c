/**! INSTRUÇÕES
*! Para compilar:
*!  - gcc -o test fatorial-bignum-thread.c -lpthread
*! Para executar:
*!  - ./test <VALUE>
*/

#include <stdio.h>
#include <stdlib.h>
#include<pthread.h> //para criar threads
#include "./lib/bignum.c"

//* Estrutura que armazena os fornece e armazena os dados de cada thread
struct data_task {
    int *p_offsets;
    int indicator_part;
    bignum partial_result;
    // int partial_result;
};

//*Tarefa executada por cada thread
void* task_thread(void *task_dt){
    struct data_task * p_data_task = (struct data_task *) task_dt;

    int_to_bignum(1, &p_data_task->partial_result);

    //Calculando os limites superiores e inferiores
    int upper_limit = p_data_task->p_offsets[p_data_task->indicator_part];

    int inferior_limit = 1;
    if(p_data_task->indicator_part > 0){
        inferior_limit = p_data_task->p_offsets[p_data_task->indicator_part -1];
    }

    printf(
        "\r\n====================================Thread %d INFO====================================\n\r",
        p_data_task->indicator_part+1
    );
    printf(
        "Indicator %d | Upper_Limit %d | Inferior_Limit %d \n\r",
        p_data_task->indicator_part, upper_limit, inferior_limit
    );

    for (int i = upper_limit; i > inferior_limit; i--)
    {
        bignum iterator;
        int_to_bignum(i, &iterator);

        bignum product;
        multiply_bignum(&p_data_task->partial_result, &iterator, &product);

        p_data_task->partial_result = product;
    }

    printf(
        "Resultado parcial do fatorial pela thread %d : ",
        p_data_task->indicator_part
    );
    print_bignum(&p_data_task->partial_result);
    printf("\n\n\r\r");
}

//Identifica o fatorial e separa em 3 para ser calculado por cada thread
void split_fatorial(int num, int *fac_offset){
    int rest;
    rest = num%3;

    if(rest == 0){
        int offset = num/3;

        fac_offset[0] = offset;
        fac_offset[1] = offset*2;
        fac_offset[2] = offset*3;
    } else {
        int offset = (int) num/3;

        fac_offset[0] = offset + rest; //Adicionando o resto para completar o número quebrado
        fac_offset[1] = fac_offset[0] + offset;
        fac_offset[2] = fac_offset[1] + offset;
    }
}


//Verifica se o número é 0, 1 ou 2. Se for, já retorna o resultado
void solve_if_small_number(int *num){
    
    if(*num == 0 || *num == 1){
        printf("Resposta: %d", 1);
        exit;
        return;
    } else if(*num == 2){
        printf("Resposta: %d", 2);
        exit;
        return;
    }
}


//Multiplica todos os resultados parciais do fatorial
void join_partial_results(bignum *a, bignum *b, bignum *c, bignum *final_result){
    bignum partial_result;
    multiply_bignum(a, b, &partial_result);
    multiply_bignum(&partial_result, c, final_result);
}

//!OBS: Primeiro argumento do argv é o numero a se calcular o fatorial
int main(int argc, char *argv[]){
    //===================================<CÁLCULO DO FATORIAL COM BIGNUM>===================================
    //pritnf("Informe o numero que deseja calcular o fatorial: ");
    //scanf("%d", &num);

    bignum bignum_n;
    int num;

    //Convertendo de char para inteiro
    num = atoi(argv[1]);
    
    printf("Calculando fatorial de: %d \n\r", num);
    
    solve_if_small_number(&num);

    int fac_offset[3];
    //Divindo o número em partes iguais para enviar para cada thread
    split_fatorial(num, fac_offset);

    //Setando os dados de cada estrutura que cada thread vai resolver
    struct data_task dt1, dt2, dt3;

    dt1.indicator_part = 0;
    dt1.p_offsets = fac_offset;

    dt2.indicator_part = 1;
    dt2.p_offsets = fac_offset;

    dt3.indicator_part = 2;
    dt3.p_offsets = fac_offset;

    // declara as tarefas por thread
    pthread_t t1, t2, t3;

    // cria as tarefas
    pthread_create(&t1, NULL, task_thread, (void *)&dt1); 
    pthread_create(&t2, NULL, task_thread, (void *)&dt2); 
    pthread_create(&t3, NULL, task_thread, (void *)&dt3); 
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    bignum final_result;
    join_partial_results(&dt1.partial_result, &dt2.partial_result, &dt3.partial_result, &final_result);
    
    printf("Resultado Final: \r\n");
    print_bignum(&final_result);

    return 0;
}