#include <stdint.h>
#include <stdio.h>

enum WeekDay {
  SUNDAY,
  MONDAY,
  TUESDAY,
  WEDNESDAY,
  THRUSDAY,
  FRIDAY,
  SATURDAY
};

int main(int argc, char *argv[])
{
  printf("sizeof(int)           == %lu bytes, %lu bits\n", sizeof(int), sizeof(int) * 8);
  printf("sizeof(unsigned int)  == %lu bytes, %lu bits\n", sizeof(unsigned int), sizeof(unsigned int) * 8);
  printf("sizeof(signed int)    == %lu bytes, %lu bits\n", sizeof(signed int), sizeof(signed int) * 8);
  printf("sizeof(long int)      == %lu bytes, %lu bits\n", sizeof(long int), sizeof(long int) * 8);
  printf("sizeof(short int)     == %lu bytes, %lu bits\n\n", sizeof(short int), sizeof(short int) * 8);

  printf("sizeof(float)         == %lu bytes, %lu bits\n\n", sizeof(float), sizeof(float) * 8);

  printf("sizeof(double)        == %lu bytes, %lu bits\n\n", sizeof(double), sizeof(double) * 8);

  printf("sizeof(void)          == %lu bytes, %lu bits\n\n", sizeof(void), sizeof(void) * 8);

  printf("sizeof(char)          == %lu bytes, %lu bits\n\n", sizeof(char), sizeof(char) * 8);

  // Putting the type you want in parenthesis before the variable name is how
  // you force it into the type you really need. The important thing though
  // is *always promote up, not down*. Don’t cast long into char unless you know
  // what you’re doing.

  printf("sizeof(int8_t)        == %lu bytes, %lu bits\n", sizeof(int8_t), sizeof(int8_t) * 8);
  printf("sizeof(uint8_t)       == %lu bytes, %lu bits\n", sizeof(uint8_t), sizeof(uint8_t) * 8);
  printf("sizeof(int16_t)       == %lu bytes, %lu bits\n", sizeof(int16_t), sizeof(int16_t) * 8);
  printf("sizeof(uint16_t)      == %lu bytes, %lu bits\n", sizeof(uint16_t), sizeof(uint16_t) * 8);
  printf("sizeof(int32_t)       == %lu bytes, %lu bits\n", sizeof(int32_t), sizeof(int32_t) * 8);
  printf("sizeof(uint32_t)      == %lu bytes, %lu bits\n", sizeof(uint32_t), sizeof(uint32_t) * 8);
  printf("sizeof(int64_t)       == %lu bytes, %lu bits\n", sizeof(int64_t), sizeof(int64_t) * 8);
  printf("sizeof(uint64_t)      == %lu bytes, %lu bits\n", sizeof(uint64_t), sizeof(uint64_t) * 8);
}
