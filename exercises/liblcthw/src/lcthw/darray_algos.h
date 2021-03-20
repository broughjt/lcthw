#ifndef darray_algos_h
#define darray_algos_h

#include <lcthw/darray.h>

typedef int (*DArray_compare) (const void *a, const void *b);

int DArray_quicksort(DArray *self, DArray_compare compare);

int DArray_heapsort(DArray *self, DArray_compare compare);

int DArray_mergesort(DArray *self, DArray_compare compare);

#endif