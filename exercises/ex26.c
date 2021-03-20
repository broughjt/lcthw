#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

/*
1. Takes any sequence of words and assumes "and" for them.
2. Takes an optional argument of -o if the parameters are meant to be "or".
3. Loads the list of allowed log files from ~/.logfind.
4. The list of file names can be anything that the glob function allows. (start with exact files, then implement glob functionality).
5. Output matching lines while scanning, try to match as fast as possible.
*/

const int MAX_DATA = 128;

int main(int argc, char *argv[])
{
  check(argc > 1, "Requires at least 1 argument");

  int offset = 1;
  bool and = strncmp(argv[1], "-o", 2);
  if (!and) {
    check(argc > 2, "Requires at least 1 word to search");
    offset = 2;
  }
  char **words = argv + offset;
  int count = argc - offset;

  // Read logfind files
  FILE *files = fopen("logfind", "r");
  check(files, "logfind could not be read");

  char buffer[MAX_DATA] = { 0 };
  char *line;
  
  while ((line = fgets(buffer, MAX_DATA, files))) {
    buffer[strcspn(buffer, "\n")] = 0;
    FILE *file = fopen(line, "r");
    check(file, "%s could not be read", line);
    printf("Searching %s:\n", line);
    while ((line = fgets(buffer, MAX_DATA, file))) {
      bool print = and;
      for (int i = 0; i < count; i++) {
        if (and != (bool)strcasestr(line, words[i])) {
          print = !print;
          break;
        }
      }
      if (print)
        printf("%s", line);
    }
    printf("\n");
  }

  return 0;
 error:
  return 1;
}
