#include <stdio.h>

void processus1() {
  while (1) {
    for (int i = 0; i < 100000000; i++)
    {
      /* do nothing */
    }
    
    printf("Hello world from P1\n");
  }
}
