#include <lcthw/debug.h>
#include <lcthw/list.h>

List *List_create()
{
  // Contiguously allocate n objects of size bytes and initialize with zeros
  return calloc(1, sizeof(List));
}

void List_destroy(List *list)
{
  LIST_FOREACH(list, first, next, current)
    if (current->previous)
      free(current->previous);

  free(list->last);
  free(list);
}

void List_clear(List *list)
{
  LIST_FOREACH(list, first, next, current)
    free(current->value);
}

void List_clear_destroy(List *list)
{
  LIST_FOREACH(list, first, next, current) {
    free(current->value);
    if (current->previous)
      free(current->previous);
  }

  free(list->last);
  free(list);
}

// Puts value at the end of the list
void List_push(List *list, void *value)
{
  ListNode *node = calloc(1, sizeof(ListNode));
  check_mem(node);

  node->value = value;

  if (list->last == NULL) {
    list->first = node;
    list->last = node;
  } else {
    list->last->next = node;
    node->previous = list->last;
    list->last = node;
  }

  list->count++;

 error:
  return;
}

// Removes last element and returns it
void *List_pop(List *list)
{
  ListNode *node = list->last;
  return node != NULL ? List_remove(list, node) : NULL;
}

// Puts value at the beginning of the list
void List_unshift(List *list, void *value)
{
  ListNode *node = calloc(1, sizeof(ListNode));
  check_mem(node);

  node->value = value;

  if (list->first == NULL) {
    list->first = node;
    list->last = node;
  } else {
    node->next = list->first;
    list->first->previous = node;
    list->first = node;
  }

  list->count++;

 error:
  return;
}

// Removes first element and returns it
void *List_shift(List *list)
{
  ListNode *node = list->first;
  return node != NULL ? List_remove(list, node) : NULL;
}

// Removes node from list wherever it is
void *List_remove(List *list, ListNode *node)
{
  void *result = NULL;

  check(list->first && list->last, "List is empty");
  check(node, "node can't be NULL");

  if (node == list->first && node == list->last) {
    list->first = NULL;
    list->last = NULL;
  } else if (node == list->first) {
    list->first = node->next;
    check(list->first != NULL, "Invalid list, somehow got a first that is NULL.");
    list->first->previous = NULL;
  } else if (node == list->last) {
    list->last = node->previous;
    check(list->last != NULL, "Invalid list, somehow got a next that is NULL.");
    list->last->next = NULL;
  } else {
    ListNode *after = node->next;
    ListNode *before = node->previous;
    after->previous = before;
    before->next = after;
  }

  list->count--;
  result = node->value;
  free(node);

 error:
  return result;
}
