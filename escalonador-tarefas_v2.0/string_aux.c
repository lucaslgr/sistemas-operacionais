#include <string.h>

//!TODO Não esta funcionando no Linux
void print(char *string)
{
  if (*string == '\0' || *string == '\r' || *string == '\n') // Base case
    return;
  printf("%c", *string);
  print(++string);
}

char *read()
{
  char *string;
  int counter = 0;
  while (1 == 1)
  {
    if (counter == 0)
      string = (char *)malloc(sizeof(char));
    else
      string = (char *)realloc(string, ((counter + 1) * sizeof(char)));

    string[counter] = getchar();

    if (string[counter] == '\r' || string[counter] == '\n' || string[counter] == '\0')
      return string;

    counter++;
  }
}