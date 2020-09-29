Nome da Tarefa:
Atividade assíncrona - 18/09
Descrição:
Implementar um simulador de escalonador de tarefas seguindo a 
estratégia FIFO (First In, First Out, ou o primeiro que entra
é o primeiro a sair). Abaixo seguem algumas sugestões:

/* 
 * Representar a tarefa como uma estrutura de dados, com
 * um identificador, um contador (equivalente ao QUANTUM atribuído
 * para cada tarefa) e um total (contador que indica quantos
 * ciclos do processador a tarefa precisa consumir).
 */

 Struct tarefa:
    - string // nome da tarefa
    - contador // inicia em 0
    - total // numero total de instruções da tarefa

/*
 * Estrutura de dados que irá armazenar as tarefas
 * operando em modo FIFO
 */
tarefa fila[5]; // fila de tarefas prontas para executar


/*
 * Função que simula a utilização do processador pela tarefa.
 * A tarefa executa no máximo 10 ciclos por vez. 
 * Cada tarefa não pode ultrapassar o total de instruções estabelecido.
 */
processa(tarefa t)
    de 1 até 10 // ciclos de processamento
        imprima ("Executando tarefa :"  t.nome)
        t.contador++
        se t.contador == total
            retorna
        
/*
 * A função principal simplesmente executa as tarefas em sequência,
 * retirando-as da fila e alocando-as no processador. Tarefas que 
 * já 
main:

    popular fila com 5 tarefas com nomes diferentes e totais diferentes
    terminadas = 0 // quantas tarefas terminaram

    while(terminadas < 5) 
        para cada tarefa t em "fila"
                processa(tarefa[t)
                se t.contador == t.total
                    remova tarefa[t] // tarefas terminadas saem da fila
                    terminadas++