#include <stdio.h>
#include <stdlib.h>

#define SIZE 2048
#define LOOP 250

int main()
{
  int i, j, count, *intPtr;

  if ((intPtr = malloc (SIZE * SIZE * sizeof(int))) == 0) {
    perror ("totally out of space");
    exit (1);
  }
  for (count=0; count<LOOP; count++)
    for (i=0; i<SIZE; i++)
      for (j=0; j<SIZE; j++)
	intPtr[i * SIZE + j] = count * i * j;

  free (intPtr);
  return 0;
}
