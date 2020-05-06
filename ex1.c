#include <sys/types.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "ex1.h"

struct block {
  unsigned int size;
  struct block *next;
  bool free;
};

#define BLOCK_SIZE sizeof(struct block)
void *head_heap = NULL;

struct block *find_free_block(struct block **last, unsigned int size, unsigned int align) {
  struct block *current = head_heap;

  while (current) {
    unsigned long start = (unsigned long) (current + 1);
    unsigned long end = start + current->size;

    while (start % align != 0 && start < end)
      start += 1;

    if (end - start >= size && current->free) {
      struct block* request_block = (struct block*) start - 1;
      request_block->size = end - start;
      request_block->next = current->next;

      if (*last) {
        (*last)->next = request_block;
        (*last)->size = (*last)->size + (unsigned long) ((void*)request_block - (void*)current);
      }

      return request_block;
    } else {
      *last = current;
      current = current->next;
    }

  }
  return NULL;
}

struct block *request_space(struct block* last, unsigned int size, unsigned int align) {
  struct block *start_block;
  start_block = sbrk(0);
  unsigned long decimal_brk = (unsigned long) (start_block + 1);

  while (decimal_brk % align != 0)
    decimal_brk += 1;

  int request = brk((void*) (decimal_brk + size));
  if (request == -1) return NULL;
  struct block* request_block = (struct block*) decimal_brk - 1;

  if (last) {
    last->size = last->size + (unsigned long) ((void*)request_block - (void*)start_block);
    last->next = request_block;
  }

  request_block->size = size;
  request_block->next = NULL;
  request_block->free = 0;

  return request_block;
}

struct block *get_block_ptr(void *ptr) {
  return (struct block*)ptr - 1;
}

void merge_free_block() {
  struct block *free_merge = head_heap;
  while (1) {
    if (free_merge == NULL) break;

    if (free_merge->free == 1) {
      struct block *new_free_block = free_merge;
      unsigned int new_size = free_merge->size;

      while (free_merge->next != NULL) {
        if (free_merge->next->free == 1){
          new_size += free_merge->next->size + BLOCK_SIZE;
          free_merge = free_merge->next;
        } else break;
      }

      new_free_block->next = free_merge->next;
      new_free_block->size = new_size;
    }

    free_merge = free_merge->next;
  }
}

void* aligned_malloc (unsigned int size, unsigned int align) {
  struct block *new_block;

  if (size <= 0) return NULL;

  if (!head_heap) { // First call.
    new_block = request_space(NULL, size, align);
    if (!new_block) return NULL;
    head_heap = new_block;

  } else {
    struct block *last = head_heap;
    new_block = find_free_block(&last, size, align);

    if (new_block == NULL) {
      new_block = request_space(last, size, align);

      if (!new_block) return NULL;

    } else new_block->free = 0;
  }

  return (new_block + 1);
}

void* aligned_free (void *ptr) {
  if (!ptr) return NULL;

  struct block* block_ptr = get_block_ptr(ptr);
  block_ptr->free = 1;

  merge_free_block();

  return ptr;
}
