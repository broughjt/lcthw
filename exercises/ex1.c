#include <stdio.h>

/* 
What does char * mean? 

Oh my goodness its a pointer of type char
Good frick does it have a size or anything?
*/
int main(int argc, char *argv[])
{
  int distance = 100;

  // Removing the distance argument reads a random value from memory, yikes!
  printf("You are %d miles away.\n", distance);

  return 0;
}
