#include <string.h>

//!TODO Não esta funcionando no Linux
void print(char *string)
{
  if (*string == '\0' || *string == '\r' || *string == '\n') // Base case
    return;
  printf("%c", *string);
  print(++string);
}

char *read_input_string(FILE *fp, size_t size)
{
  //The size is extended by the input with the value of the provisional
  char *str;
  int ch;
  size_t len = 0;
  str = realloc(NULL, sizeof(char) * size); //size is start size
  if (!str)
    return str;
  while (EOF != (ch = fgetc(fp)) && ch != '\n')
  {
    str[len++] = ch;
    if (len == size)
    {
      str = realloc(str, sizeof(char) * (size += 16));
      if (!str)
        return str;
    }
  }
  str[len++] = '\0';

  return realloc(str, sizeof(char) * len);
}

// char *read()
// {
//   char *string;
//   int counter = 0;
//   while (1 == 1)
//   {
//     if (counter == 0)
//       string = (char *)malloc(sizeof(char));
//     else
//       string = (char *)realloc(string, ((counter + 1) * sizeof(char)));

//     string[counter] = getchar();

//     if (string[counter] == '\r' || string[counter] == '\n' || string[counter] == '\0')
//       return string;

//     counter++;
//   }
// }