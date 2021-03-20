#ifndef _radixmap_h
#define _radixmap_h

#include <stddef.h>
#include <stdint.h>

typedef union RMElement {
  uint64_t raw;
  struct {
    uint32_t key;
    uint32_t value;
  } data;
} RMElement;

typedef struct RadixMap {
  size_t length;
  size_t capacity;
  uint32_t counter;
  RMElement *data;
  RMElement *temp;
} RadixMap;

RadixMap *RadixMap_create(size_t capacity);

void RadixMap_destroy(RadixMap *self);

void RadixMap_sort(RadixMap *self);

RMElement *RadixMap_find(RadixMap *self, uint32_t key);

int RadixMap_add(RadixMap *self, uint32_t key, uint32_t value);

int RadixMap_delete(RadixMap *self, RMElement *element);

#endif