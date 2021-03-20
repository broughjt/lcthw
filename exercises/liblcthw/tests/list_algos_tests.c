#include "minunit.h"
#include <lcthw/list_algos.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

char *values[] = { "XXXX", "1234", "abcd", "xjvef", "NDSS" };

#define NUM_VALUES 5

List *create_words()
{
  int i = 0;
  List *words = List_create();

  for (i = 0; i < NUM_VALUES; i++)
    List_push(words, values[i]);

  return words;
}

int is_sorted(List *words)
{
  LIST_FOREACH(words, first, next, current)
    if (current->next && strcmp(current->value, current->next->value) > 0) {
      debug("%s %s", (char *)current->value, (char *)current->next->value);
      return 0;
    }

  return 1;
}

char *test_bubble_sort()
{
  List *words = create_words();

  // Should work on a list that needs sorting
  int rc = List_bubble_sort(words, (List_compare) strcmp);
  mu_assert(rc == 0, "Bubble sort failed.");
  mu_assert(is_sorted(words), "Words are not sorted after bubble sort.");

  // Should work on an already sorted list
  rc = List_bubble_sort(words, (List_compare) strcmp);
  mu_assert(rc == 0, "Bubble sort on already sorted data failed.");
  mu_assert(is_sorted(words), "Words should be sorted if already bubble sorted.");

  List_destroy(words);

  // Should work on an empty list
  words = List_create(words);
  rc = List_bubble_sort(words, (List_compare) strcmp);
  mu_assert(rc == 0, "Bubble sort failed on empty list.");
  mu_assert(is_sorted(words), "Words should be sorted if empty.");

  List_destroy(words);

  return NULL;
}

char *test_merge_sort()
{
  List *words = create_words();

  // Should work on a list that needs sorting
  List *list1 = List_merge_sort(words, (List_compare) strcmp);
  mu_assert(is_sorted(list1), "Words are not sorted after merge sort.");

  List *list2 = List_merge_sort(list1, (List_compare) strcmp);
  mu_assert(is_sorted(list1), "Should still be sorted after merge sort.");

  List_destroy(list2);
  List_destroy(list1);

  List_destroy(words);

  return NULL;
}

char *all_tests()
{
  mu_suite_start();

  mu_run_test(test_bubble_sort);
  mu_run_test(test_merge_sort);

  return NULL;
}

RUN_TESTS(all_tests);
