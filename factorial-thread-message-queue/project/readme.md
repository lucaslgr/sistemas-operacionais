### O que deve ser feito?

Adaptar o programa que faz o cálculo de fatorial de forma concorrente para que utilize filas de mensagens. O programa resultante deve ser composto por pelo menos 3 processos (1 servidor 2 trabalhadores): o servidor comunica-se com os trabalhadores utilizando filas de mensagens, solicitando que cada trabalhador faça as múltiplicações parciais de uma sub-faixa do fatorial (assim como as threads faziam no programa original). Ao fim, o servidor consolida os resultados parciais e apresenta o resultado geral.

Os processos podem ser executados de forma independente. Isto é, não é preciso implementar o uso de fork (inexistente no windows).

Enviar o código fonte do processo servidor e dos processos trabalhadores.
