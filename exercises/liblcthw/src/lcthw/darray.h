#ifndef _DArray_h
#define _DArray_h

#include <stdlib.h>
#include <assert.h>

#include <lcthw/debug.h>

typedef struct DArray {
  int length;
  int capacity;
  size_t element_size;
  size_t expand_rate;
  void **data;
} DArray;

DArray *DArray_create(size_t element_size, size_t capacity);

void DArray_destroy(DArray *self);

void DArray_clear(DArray *self);

int DArray_expand(DArray *self);

int DArray_contract(DArray *self);

int DArray_push(DArray *self, void *value);

void *DArray_pop(DArray *self);

void DArray_clear_destroy(DArray *self);

#define DArray_last(A) ((A)->contents[(A)->end - 1])
#define DArray_first(A) ((A)->contents[0])
#define DArray_length(A) ((A)->length)
#define DArray_capacity(A) ((A)->capacity)

#define DEFAULT_EXPAND_RATE 300

static inline void DArray_set(DArray *self, int index, void *value)
{
  check(index < self->capacity, "darray attempt to set past max");
  if (index > self->length)
    self->length = index;
  self->data[index] = value;

 error:
  return;
}

static inline void *DArray_get(DArray *self, int index)
{
  check(index < self->capacity, "darray attempt to get past max");
  return self->data[index];

 error:
  return NULL;
}

static inline void *DArray_remove(DArray *self, int index)
{
  void *value = self->data[index];

  self->data[index] = NULL;

  return value;
}

static inline void *DArray_new(DArray *self)
{
  check(self->element_size > 0, "Can't use DArray_new on 0 size darrays");
  return calloc(1, self->element_size);

 error:
  return NULL;
}

#define DArray_free(E) free((E))

#endif
