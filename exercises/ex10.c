#include <stdio.h>

char lower(char c)
{
  return ((c >= 65) && (c <= 90)) ? c + 32 : c;
}

int main(int argc, char *argv[])
{
  if (argc != 2) {
    printf("ERROR: You need one argument.\n");
    return 1;
  }

  int i = 0;
  for (i = 0; argv[1][i] != '\0'; i++) {
    char letter = argv[1][i];
    switch (lower(letter)) {
    case 'a':
    case 'e':
    case 'i':
    case 'o':
    case 'u':
      printf("%d: '%c'\n", i, letter);
      break;
    case 'y':
      if (i > 2) {
	printf("%d: '%c'\n", i, letter);
	break;
      }
    default:
      printf("%d: %c is not a vowel\n", i, letter);
    }
  }

  return 0;
}
