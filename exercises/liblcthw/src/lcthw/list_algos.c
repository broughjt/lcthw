#include <stdbool.h>

#include <lcthw/list_algos.h>
#include <lcthw/debug.h>

inline void swap(ListNode *a, ListNode *b)
{
  void *t = a->value;
  a->value = b->value;
  b->value = t;
}

int List_bubble_sort(List *list, List_compare compare)
{
  bool swapped = false;

  if (List_count(list) <= 1)
    return 0;

  do {
    swapped = false;
    LIST_FOREACH(list, first, next, current) {
      if (current->next && compare(current->value, current->next->value) > 0) {
        swap(current, current->next);
        swapped = true;
      }
    }
  } while (swapped);

  return 0;
}

inline List *merge(List *left, List *right, List_compare compare)
{
  List *result = List_create();
  void *value = NULL;

  while (List_count(left) > 0 || List_count(right) > 0) {
    if (List_count(left) > 0 && List_count(right) > 0)
      value = List_shift(compare(List_first(left), List_first(right)) <= 0 ? left : right);
    else if (List_count(left) > 0)
      value = List_shift(left);
    else if (List_count(right) > 0)
      value = List_shift(right);
    List_push(result, value);
  }

  return result;
}

List *List_merge_sort(List *list, List_compare compare)
{
  List *result = NULL;
  int count = List_count(list);

  if (count <= 1)
    return list;

  List *left = List_create();
  List *right = List_create();
  int middle = count / 2;

  LIST_FOREACH(list, first, next, current) {
    List_push(middle > 0 ? left : right, current->value);
    middle--;
  }

  List *sorted_left = List_merge_sort(left, compare);
  List *sorted_right = List_merge_sort(right, compare);

  if (sorted_left != left)
    List_destroy(left);
  if (sorted_right != right)
    List_destroy(right);

  result = merge(sorted_left, sorted_right, compare);

  List_destroy(sorted_left);
  List_destroy(sorted_right);

  return result;
}
