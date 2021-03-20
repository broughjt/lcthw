#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void die(const char *message)
{
  if (errno) {
    perror(message);
  } else {
    printf("Error: %s\n", message);
  }

  exit(1);
}

typedef int (*comparison) (int a, int b);

int *bubble_sort(int *numbers, int count, comparison compare)
{
  int temp = 0;
  int i = 0;
  int j = 0;
  int *target = malloc(count * sizeof(int));

  if (!target)
    die("Memory error.");

  memcpy(target, numbers, count * sizeof(int));

  for (i = 0; i < count; i++) {
    for (j = 0; j < count - 1; j++) {
      if (compare(target[j], target[j + 1]) > 0) {
	temp = target[j + 1];
	target[j + 1] = target[j];
	target[j] = temp;
      }
    }
  }

  return target;
}

int sorted_order(int a, int b)
{
  return (a > b) - (a < b);
}

int reverse_order(int a, int b)
{
  return (a < b) - (a > b);
}

int strange_order(int a, int b)
{
  return (a == 0 || b == 0) ? 0 : a % b;
}

void test_sorting(int *numbers, int count, comparison compare)
{
  int i = 0;
  int *sorted = bubble_sort(numbers, count, compare);

  if (!sorted)
    die("Failed to sort as requested.");

  for (i = 0; i < count; i++)
    printf("%d ", sorted[i]);
  printf("\n");

  free(sorted);
}

int main (int argc, char *argv[])
{
  if (argc < 2)
    die("USAGE: ex18 4 3 1 5 6");

  int count = argc - 1;
  int i = 0;
  char **inputs = argv + 1; // Whoa, alright there then. This is how the C folks do things around here

  int *numbers = malloc(count * sizeof(int));
  if (!numbers)
    die("Memory error.");

  for (i = 0; i < count; i++)
    numbers[i] = atoi(inputs[i]);

  for (i = 0; i < count; i++)
    printf("%d ", numbers[i]);
  printf("\n\n");

  test_sorting(numbers, count, sorted_order);
  test_sorting(numbers, count, reverse_order);
  test_sorting(numbers, count, strange_order);

  free(numbers);

  return 0;
}