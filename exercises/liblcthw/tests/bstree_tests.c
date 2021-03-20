#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include "minunit.h"
#include <lcthw/bstree.h>
#include <lcthw/bstrlib.h>

BSTree *map = NULL;
static int traverse_called = 0;
struct tagbstring test1 = bsStatic("test data 1");
struct tagbstring test2 = bsStatic("test data 2");
struct tagbstring test3 = bsStatic("test data 3");
struct tagbstring expect1 = bsStatic("THE VALUE 1");
struct tagbstring expect2 = bsStatic("THE VALUE 2");
struct tagbstring expect3 = bsStatic("THE VALUE 3");

static int traverse_good_callback(BSTreeNode *node)
{
  debug("key: %s", bdata((bstring) node->key));
  traverse_called++;

  return 0;
}

static int traverse_fail_callback(BSTreeNode *node)
{
  debug("key: %s", bdata((bstring) node->key));
  traverse_called++;

  return traverse_called == 2;
}

char *test_create()
{
  map = BSTree_create(NULL);
  mu_assert(map != NULL, "Failed to create map");

  return NULL;
}

char *test_destroy()
{
  BSTree_destroy(map);

  return NULL;
}

char *test_get_set()
{
  BSTreeNode *node = BSTree_set(map, &test1, &expect1);
  mu_assert(node != NULL, "Failed to set &test1");
  bstring value = BSTree_get(map, &test1);
  mu_assert(value == &expect1, "Wrong value for test1");

  node = BSTree_set(map, &test2, &expect2);
  mu_assert(node != NULL, "Failed to set test2");
  value = BSTree_get(map, &test2);
  mu_assert(value == &expect2, "Wrong value for test2");

  node = BSTree_set(map, &test3, &expect3);
  mu_assert(node != NULL, "Failed to set test3");
  value = BSTree_get(map, &test3);
  mu_assert(value == &expect3, "Wrong value for test3");

  return NULL;
}

char *test_traverse()
{
  int result = BSTree_traverse(map, traverse_good_callback);
  mu_assert(result == 0, "Failed to traverse");
  mu_assert(traverse_called == 3, "Wrong count traverse");

  traverse_called = 0;
  result = BSTree_traverse(map, traverse_fail_callback);
  mu_assert(result == 1, "Failed to traverse");
  mu_assert(traverse_called == 2, "Wrong count traverse for fail");

  return NULL;
}

char *test_delete()
{
  bstring deleted = (bstring) BSTree_delete(map, &test1);
  mu_assert(deleted != NULL, "Got NULL on delete.");
  mu_assert(deleted == &expect1, "Should get test1");
  bstring value = BSTree_get(map, &test1);
  mu_assert(value == NULL, "Should delete.");

  deleted = (bstring) BSTree_delete(map, &test1);
  mu_assert(deleted == NULL, "Should get NULL on delete");

  deleted = (bstring) BSTree_delete(map, &test2);
  mu_assert(deleted != NULL, "Got NULL on delete.");
  mu_assert(deleted == &expect2, "Should get test2");
  value = BSTree_get(map, &test2);
  mu_assert(value == NULL, "Should delete.");

  deleted = (bstring) BSTree_delete(map, &test3);
  mu_assert(deleted != NULL, "Got NULL on delete.");
  mu_assert(deleted == &expect3, "Should get test3");
  value = BSTree_get(map, &test3);
  mu_assert(value == NULL, "Should delete.");

  // test deleting non-existent stuff
  deleted = (bstring) BSTree_delete(map, &test3);
  mu_assert(deleted == NULL, "Should get NULL");

  return NULL;
}

char *test_fuzzing()
{
  BSTree *store = BSTree_create(NULL);
  bstring numbers[100] = { NULL };
  bstring data[100] = { NULL };
  srand((unsigned int) time(NULL));

  for (int i = 0; i < 100; i++) {
    int number = rand();
    numbers[i] = bformat("%d", number);
    data[i] = bformat("data %d", number);
    BSTree_set(store, numbers[i], data[i]);
  }

  for (int i = 0; i < 100; i++) {
    bstring value = BSTree_delete(store, numbers[i]);
    mu_assert(value == data[i], "Failed to delete the right number");
    mu_assert(BSTree_delete(store, numbers[i]) == NULL, "Should get nothing");

    for (int j = i + 1; j < 99 - i; j++) {
      bstring value = BSTree_get(store, numbers[j]);
      mu_assert(value == data[j], "Failed to get the right number");
    }

    bdestroy(value);
    bdestroy(numbers[i]);
  }

  BSTree_destroy(store);

  return NULL;
}

char *all_tests()
{
  mu_suite_start();

  mu_run_test(test_create);
  mu_run_test(test_get_set);
  mu_run_test(test_traverse);
  mu_run_test(test_delete);
  mu_run_test(test_destroy);
  mu_run_test(test_fuzzing);

  return NULL;
}

RUN_TESTS(all_tests);