#include <sys/types.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>
#include "ex1.h"

#define WORD_SIZE 4

void* aligned_malloc (unsigned int size, unsigned int align) {
  void* current_brk = sbrk(0);
  unsigned long long decimal_brk = (unsigned long long) current_brk + WORD_SIZE;

  while (decimal_brk % align != 0)
    decimal_brk += 1;

  void * new_brk = (void*) (decimal_brk + size);

  if (brk(new_brk) == 0) {
    unsigned int * alloc_size = (void*) (decimal_brk - WORD_SIZE);
    *alloc_size = size;

    return (void*) decimal_brk;
  }

  return NULL;
}

void* aligned_free (void *ptr) {
  void* start_free = ptr - WORD_SIZE;
  unsigned int *size = (unsigned int*) start_free;
  *size += WORD_SIZE;

  void* end_free = start_free + *size;

  while (start_free != end_free) {
    char *b = start_free;
    *b = 0;
    start_free += 1;
  }

  return ptr;
}
