#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "ex1.h"

int main() {
  printf("%s\n", "This below result uses align_malloc...");
  int *var = (int*) aligned_malloc(5*sizeof(int), 64);

  for (int i = 0; i < 5; i++) {
    var[i] = i;
  }

  for (int i = 0; i < 5; i++) {
    printf("%d\n", var[i]);
  }

  printf("Address: %p\n", (void*)var);

  printf("%s\n", "Now this is aligned_free...");
  aligned_free(var);

  printf("Alloc a new other array...\n");
  int *var_1 = (int*) aligned_malloc(2*sizeof(int), 64);
  printf("Address: %p\n", (void*)var_1);

  return 0;
}
