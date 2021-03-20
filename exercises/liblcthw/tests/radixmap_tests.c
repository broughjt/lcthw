#include "minunit.h"
#include <lcthw/radixmap.h>
#include <time.h>

static int make_random(RadixMap *map)
{
  size_t value = 0;

  for (value = 0; value < map->capacity - 1; value++) {
    uint32_t key = (uint32_t) (rand() | (rand() << 16));
    int result = RadixMap_add(map, key, value);
    check(result == 0, "Failed to add key %u", key);
  }

  return value;

error:
  return 0;
}

static int check_order(RadixMap *map)
{
  RMElement d1, d2;

  for (unsigned int i = 0; map->length > 0 && i < map->length - 1; i++) {
    d1 = map->data[i];
    d2 = map->data[i + 1];

    if (d1.data.key > d2.data.key) {
      debug("FAIL: i=%u, key: %u, value: %u, equals max? %d\n", i, d1.data.key, d1.data.value, d2.data.key == UINT32_MAX);
      return 0;
    }
  }

  return 1;
}

static int test_search(RadixMap *map)
{
  RMElement *c = NULL;
  RMElement *f = NULL;

  for (unsigned i = map->length / 2; i < map->length; i++) {
    c = &map->data[i];
    f = RadixMap_find(map, c->data.key);

    check(f != NULL, "Didn't find %u at %u", c->data.key, i);
    check(f->data.key == c->data.key, "Got the wrong result: %p:%u looking for %u at %u", f, f->data.key, c->data.key, i);
  }

  return 1;

error:
  return 0;
}

static char *test_operations()
{
  size_t n = 200;

  RadixMap *map = RadixMap_create(n);
  mu_assert(map != NULL, "Failed to make the map");
  mu_assert(make_random(map), "Didn't make a random fake radix map");

  RadixMap_sort(map);
  mu_assert(check_order(map), "Failed to properly sort the RadixMap");

  mu_assert(test_search(map), "Failed the search test");
  mu_assert(check_order(map), "RadixMap didn't stay sorted after search");

  while (map->length > 0) {
    RMElement *element = RadixMap_find(map, map->data[map->length / 2].data.key);
    mu_assert(element != NULL, "Should get a result");

    size_t old_length = map->length;

    mu_assert(RadixMap_delete(map, element) == 0, "Didn't delete it");
    mu_assert(old_length - 1 == map->length, "Wrong size after delete");

    // test that the end is now the old value,
    // but uint32 max so it trails off
    mu_assert(check_order(map), "RadixMap didn't stay sorted after delete");
  }

  RadixMap_destroy(map);

  return NULL;
}

char *all_tests()
{
  mu_suite_start();
  srand(time(NULL));

  mu_run_test(test_operations);

  return NULL;
}

RUN_TESTS(all_tests);