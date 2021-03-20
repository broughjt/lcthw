#include <stdio.h>

int main(int argc, char *argv[])
{
  printf("%d", NULL || 3);

  return 0;
}