#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "./lib/bignum.c"

#define NUM_THREAD 5

struct dados_t
{
	int num_inicial;
	int num_final;
	bignum result_parcial;
};

void *calculaFatorial(void *dados)
{
	struct dados_t *dd = (struct dados_t *)dados;

	// printf("=======\n\rInicial: %d, Final: %d\n\r=========", dd->num_inicial, dd->num_final);

	bignum bignum_n, bignum_result, bignum_partial_result;

	int_to_bignum(dd->num_final, &bignum_n);
	int_to_bignum(1, &bignum_result);

	int limite_superior = dd->num_final;

	while (limite_superior >= dd->num_inicial)
	{
		bignum_partial_result = bignum_result;

		multiply_bignum(&bignum_partial_result, &bignum_n, &bignum_result);
		limite_superior--;
		int_to_bignum(limite_superior, &bignum_n);
	}
	dd->result_parcial = bignum_result;

	printf("\n\rResultado parcial de cada thread onde n_incial: %d e n_final %d :", dd->num_inicial, dd->num_final);
	print_bignum(&dd->result_parcial);
	printf("\n\r");
}

int main()
{
	int n;
	printf("Digite um valor para ser calculado o fatorial: ");
	scanf("%d", &n);

	clock_t start, end;

	if (n == 1 || n == 0)
	{
		printf("Fatorial -- 1");
		return 0;
		exit;
	}

	if (NUM_THREAD > n)
	{
		printf("O numero de thread n�o pode ser maior que n");
		return 0;
		exit;
	}

	// declara as tarefas
	pthread_t *p_threads = (pthread_t *)malloc(sizeof(pthread_t) * NUM_THREAD);

	struct dados_t *vetor_struct = (struct dados_t *)malloc(sizeof(struct dados_t) * NUM_THREAD);

	start = clock();

	// Divisão do numero
	int resto = n % NUM_THREAD;

	if (resto == 0)
	{
		int passo = n / NUM_THREAD;

		int k;
		for (k = 0; k < NUM_THREAD; k++)
		{

			int n_min, n_max;

			if (k == 0)
			{
				n_min = 1;
				n_max = passo;
			}
			else
			{
				n_min = (vetor_struct - k)->num_final + 1;
				n_max = n_min + passo;
			}
			(vetor_struct + k)->num_inicial = n_min;
			(vetor_struct + k)->num_final = n_max;
		}
	}
	else
	{
		int passo = n / NUM_THREAD;
		int i;
		for (i = 0; i < NUM_THREAD; i++)
		{

			int n_min = 0, n_max = 0;

			if (i == 0)
			{
				n_min = 1;
				n_max = passo;
				//printf("para i=0 nmin: %d, nmax: %d \n", n_min, n_max);
				exit;
			}
			else if (i == (NUM_THREAD - 1))
			{
				n_min = (vetor_struct + (i - 1))->num_final + 1;
				n_max = (vetor_struct + (i - 1))->num_final + passo + resto;
				// printf("para i=%d nmin: %d, nmax: %d , num_final_anterior %d\n", i, n_min, n_max, (vetor_struct + i - 1)->num_final);
			}
			else
			{
				n_min = (vetor_struct + (i - 1))->num_final + 1;
				n_max = (vetor_struct + (i - 1))->num_final + passo;
				// printf("para i=%d nmin: %d, nmax: %d , num_final_anterior %d\n", i, n_min, n_max, (vetor_struct + i - 1)->num_final);
			}
			//printf(" nmin: %d, nmax: %d , num_final_anterior %d\n", n_min, n_max);
			(vetor_struct + i)->num_inicial = n_min;
			(vetor_struct + i)->num_final = n_max;
		}
	}
	printf("0 num inicial %d, num final %d\r\n", (vetor_struct)->num_inicial, (vetor_struct)->num_final);
	printf("1 num inicial %d, num final %d\r\n", (vetor_struct + 1)->num_inicial, (vetor_struct + 1)->num_final);
	printf("2 num inicial %d, num final %d\r\n", (vetor_struct + 2)->num_inicial, (vetor_struct + 2)->num_final);

	// cria as tarefas
	int i;
	for (i = 0; i < NUM_THREAD; i++)
	{
		pthread_create((p_threads + i), NULL, calculaFatorial, (void *)(vetor_struct + i));
	}
	for (i = 0; i < NUM_THREAD; i++)
	{
		pthread_join(*(p_threads + i), NULL);
	}

	bignum result;
	for (i = 0; i < (NUM_THREAD - 1); i++)
	{
		// printf("*************resultado parcial****************:");
		// print_bignum(&(vetor_struct + i)->result_parcial);

		bignum aux;
		if (i == 0)
		{
			multiply_bignum(&(vetor_struct + i)->result_parcial, &(vetor_struct + i + 1)->result_parcial, &result);
			aux = result;
			printf("&&&&");
			print_bignum(&result);
		}
		else
		{
			multiply_bignum(&aux, &(vetor_struct + (i + 1))->result_parcial, &result);
			aux = result;
		}
	}

	end = clock();
	double tmp = (double)(end - start);
	tmp = tmp / (double)CLOCKS_PER_SEC;
	printf("\nTempo decorrido %f\n", tmp);

	print_bignum(&(vetor_struct + i)->result_parcial);
	// mostra os valores dos dados modificados
	printf("O valor do fatorial eh:");
	print_bignum(&result);

	return 0;
}
