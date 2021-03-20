#include <lcthw/darray.h>
#include <lcthw/debug.h>

DArray *DArray_create(size_t element_size, size_t capacity)
{
  DArray *self = malloc(sizeof(DArray));
  check_mem(self);
  self->capacity = capacity;
  check(self->capacity > 0, "You must set an initial max > 0");

  self->data = calloc(capacity, sizeof(void *));
  check_mem(self->data);

  self->length = 0;
  self->element_size = element_size;
  self->expand_rate = DEFAULT_EXPAND_RATE;

  return self;

 error:
  if (self->data)
    free(self->data);
  if (self)
    free(self);

  return NULL;
}

void DArray_clear(DArray *self)
{
  if (self->element_size > 0)
    for (int i = 0; i < self->capacity; i++)
      if (self->data[i] != NULL)
        free(self->data[i]);
}

static inline int DArray_resize(DArray *self, size_t capacity)
{
  self->capacity = capacity;
  check(self->capacity > 0, "The capacity must be > 0");

  void *data = realloc(self->data, self->capacity * sizeof(void *));
  check_mem(data);

  self->data = data;

  return 0;

 error:
  return -1;
}

int DArray_expand(DArray *self)
{
  size_t capacity = self->capacity;
  int result = DArray_resize(self, self->capacity + self->expand_rate);
  check(result == 0, "Failed to expand array to new size: %d", self->capacity + (int)self->expand_rate);
  memset(self->data + capacity, 0, self->expand_rate + 1);

  return 0;

 error:
  return -1;
}

int DArray_contract(DArray *self)
{
  int capacity = self->length < (int)self->expand_rate ? (int)self->expand_rate : self->length;

  return DArray_resize(self, capacity + 1);
}

void DArray_destroy(DArray *self)
{
  if (self) {
    if (self->data)
      free(self->data);
    free(self);
  }
}

void DArray_clear_destroy(DArray *self)
{
  DArray_clear(self);
  DArray_destroy(self);
}

int DArray_push(DArray *self, void *value)
{
  self->data[self->length++] = value;

  return DArray_length(self) >= DArray_capacity(self) ? DArray_expand(self) : 0;
}

void *DArray_pop(DArray *self)
{
  check(self->length - 1 >= 0, "Attempt to pop from empty array.");

  void *value = DArray_remove(self, --self->length);

  if (DArray_length(self) > (int)self->expand_rate && DArray_length(self) % self->expand_rate)
    DArray_contract(self);

  return value;

 error:
  return NULL;
}
