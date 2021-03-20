#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <lcthw/radixmap.h>
#include <lcthw/debug.h>

RadixMap *RadixMap_create(size_t capacity)
{
  RadixMap *self = calloc(1, sizeof(RadixMap));
  check_mem(self);

  self->data = calloc(capacity + 1, sizeof(RMElement));
  check_mem(self->data);

  self->temp = calloc(capacity + 1, sizeof(RMElement));
  check_mem(self->temp);

  self->capacity = capacity;
  self->length = 0;

  return self;
error:
  RadixMap_destroy(self);
  return NULL;
}

void RadixMap_destroy(RadixMap *self)
{
  if (self) {
    free(self->data);
    free(self->temp);
    free(self);
  }
}

#define ByteOf(x, y) (((uint8_t *)x)[(y)])

static inline void radix_sort(
  short offset,
  uint64_t capacity,
  uint64_t *source,
  uint64_t *destination
)
{
  uint64_t count[256] = { 0 };
  uint64_t *cp = NULL;
  uint64_t *sp = NULL;
  uint64_t *length = NULL;
  uint64_t s = 0;
  uint64_t c = 0;

  // count occurences of every byte value
  for (sp = source, length = source + capacity; sp < length; sp++)
    count[ByteOf(sp, offset)]++;

  // transform count into index by summing
  // elements and storing into same array
  for (s = 0, cp = count, length = count + 256; cp < length; cp++) {
    c = *cp;
    *cp = s;
    s += c;
  }

  // fill destination with the right values in the right place
  for (sp = source, length = source + capacity; sp < length; sp++) {
    cp = count + ByteOf(sp, offset);
    destination[*cp] = *sp;
    ++(*cp);
  }
}

void RadixMap_sort(RadixMap *self)
{
  uint64_t *temp = &self->data[0].raw;
  uint64_t *data = &self->temp[0].raw;

  radix_sort(0, self->length, data, temp);
  radix_sort(1, self->length, temp, data);
  radix_sort(2, self->length, data, temp);
  radix_sort(3, self->length, temp, data);
}

RMElement *RadixMap_find(RadixMap *self, uint32_t key)
{
  int low = 0;
  int high = self->length - 1;
  RMElement *data = self->data;

  while (low <= high) {
    int middle = low + (high - low) / 2;
    uint32_t candidate = data[middle].data.key;

    if (key < candidate)
      high = middle - 1;
    else if (key > candidate)
      low = middle + 1;
    else
      return &data[middle];
  }

  return NULL;
}

int RadixMap_add(RadixMap *self, uint32_t key, uint32_t value)
{
  check(key < UINT32_MAX, "Key can't be equal to UINT32_MAX");

  RMElement element = { .data = { .key = key, .value = value } };
  check(self->length + 1 < self->capacity, "RadixMap is full");

  self->data[self->length++] = element;

  RadixMap_sort(self);

  return 0;
error:
  return -1;
}

int RadixMap_delete(RadixMap *self, RMElement *element)
{
  check(self->length > 0, "There is nothing to delete");
  check(element != NULL, "Can't delete a NULL element");

  element->data.key = UINT32_MAX;

  if (self->length > 1)
    // Don't bother resorting a map of length 1
    RadixMap_sort(self);

  self->length--;

  return 0;
error:
  return -1;
}