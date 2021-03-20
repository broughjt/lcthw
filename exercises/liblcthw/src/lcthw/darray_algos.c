#include <lcthw/darray_algos.h>
#include <stdlib.h>

int DArray_quicksort(DArray *self, DArray_compare compare)
{
    qsort(self->data, DArray_length(self), sizeof(void *), compare);
    return 0;
}

int DArray_heapsort(DArray *self, DArray_compare compare)
{
    return heapsort(self->data, DArray_length(self), sizeof(void *), compare);
}

int DArray_mergesort(DArray *self, DArray_compare compare)
{
    return mergesort(self->data, DArray_length(self), sizeof(void *), compare);
}