#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 16

int main()
{
  char *data1, *data2, *tmp;
  int i;

  data1 = malloc (SIZE);
  data2 = malloc (SIZE);
  tmp = data1;
  printf ("Please input your eos username: ");
  scanf ("%s", data1);
  for (i=0; i<SIZE; i++)
    data2[i] = *(tmp+i);
  
  printf ("data2 :%s:\n", data2);
  printf ("tmp :%s:\n", tmp);
  free (data1);
  free (data2);
  return 0;
}
