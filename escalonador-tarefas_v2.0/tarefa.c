struct tarefa
{
  //Nome da tarefa
  char *nome;
  // Inicia em 0
  int contador;
  // Numero total de instruções da tarefa
  int total_instrucoes;
};

//Construtor da fila circular
struct tarefa tarefa_construct(char *nome, int total_instrucoes)
{
  struct tarefa task;
  task.nome = nome;
  task.contador = 0;
  task.total_instrucoes = total_instrucoes;

  return task;
}