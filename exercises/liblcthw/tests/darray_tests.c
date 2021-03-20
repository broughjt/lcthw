#include "minunit.h"
#include <lcthw/darray.h>

static DArray *array = NULL;
static int *value1 = NULL;
static int *value2 = NULL;

char *test_create()
{
  array = DArray_create(sizeof(int), 100);
  mu_assert(array != NULL, "DArray create failed");
  mu_assert(array->data != NULL, "data is wrong in darray");
  mu_assert(array->length == 0, "length isn't at the right spot");
  mu_assert(array->element_size == sizeof(int), "element_size is wrong.");
  mu_assert(array->capacity == 100, "wrong capacity on initial size");

  return NULL;
}

char *test_destroy()
{
  DArray_destroy(array);

  return NULL;
}

char *test_new()
{
  value1 = DArray_new(array);
  mu_assert(value1 != NULL, "failed to make a new element");

  value2 = DArray_new(array);
  mu_assert(value2 != NULL, "failed to make a new element");

  return NULL;
}

char *test_set()
{
  DArray_set(array, 0, value1);
  DArray_set(array, 1, value2);

  return NULL;
}

char *test_get()
{
  mu_assert(DArray_get(array, 0) == value1, "Wrong first value");
  mu_assert(DArray_get(array, 1) == value2, "Wrong second value");

  return NULL;
}

char *test_remove()
{
  int *value = DArray_remove(array, 0);
  mu_assert(value != NULL, "Should not get NULL");
  mu_assert(*value == *value1, "Should get the first value");
  mu_assert(DArray_get(array, 0) == NULL, "Should be gone");
  DArray_free(value);

  value = DArray_remove(array, 1);
  mu_assert(value != NULL, "Should not get NULL");
  mu_assert(*value == *value2, "Should get the first value");
  mu_assert(DArray_get(array, 1) == NULL, "Should be gone");
  DArray_free(value);

  return NULL;
}

char *test_expand_contract()
{
  int old_capacity = array->capacity;
  DArray_expand(array);
  mu_assert((unsigned int)array->capacity == old_capacity + array->expand_rate, "Wrong size after expand");

  DArray_contract(array);
  mu_assert((unsigned int)array->capacity == array->expand_rate + 1, "Should stay at the expand_rate at least.");

  DArray_contract(array);
  mu_assert((unsigned int)array->capacity == array->expand_rate + 1, "Should stay at the expand_rate at least.");

  return NULL;
}

char *test_push_pop()
{
  int i = 0;
  for (i = 0; i < 1000; i++) {
    int *value = DArray_new(array);
    *value = i * 333;
    DArray_push(array, value);
  }

  mu_assert(array->capacity == 1201, "Wrong max size.");

  for (i = 999; i > 0; i--) {
    int *value = DArray_pop(array);
    mu_assert(value != NULL, "Shouldn't get a NULL");
    mu_assert(*value == i * 333, "Wrong value");
    DArray_free(value);
  }

  return NULL;
}

char *all_tests()
{
  mu_suite_start();

  mu_run_test(test_create);
  mu_run_test(test_new);
  mu_run_test(test_set);
  mu_run_test(test_get);
  mu_run_test(test_remove);
  mu_run_test(test_expand_contract);
  mu_run_test(test_push_pop);
  mu_run_test(test_destroy);

  return NULL;
}

RUN_TESTS(all_tests);
