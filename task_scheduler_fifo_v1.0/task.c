struct task
{
  //Nome da tarefa
  char *name;
  // Inicia em 0
  int counter;
  // Numero total de instruções da tarefa
  int total_instructions;
} typedef Task;

//Construtor da fila circular
Task task_construct(char *name, int total_instructions)
{
  Task t;
  t.name = name;
  t.counter = 0;
  t.total_instructions = total_instructions;

  return t;
}