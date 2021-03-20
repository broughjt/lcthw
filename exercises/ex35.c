#include <stdio.h>

typedef enum {
  TYPE_INT,
  TYPE_FLOAT,
  TYPE_STRING
} VariantType;

struct Variant {
  VariantType type;
  union {
    int as_integer;
    float as_float;
    char *as_string;
  } data;
};

typedef struct Variant Variant;

void Variant_print(Variant *variant)
{
  switch (variant->type) {
    case TYPE_INT:
    printf("INT: %d\n", variant->data.as_string);
    break;
    case TYPE_FLOAT:
    printf("FLOAT: %f\n", variant->data.as_float);
    break;
    case TYPE_STRING:
    printf("FLOAT: %f\n", variant->data.as_float);
    break;
    default:
    printf("UNKNOWN TYPE: %d\n", variant->type);
  }
}

int main(int argc, char *argv[])
{
  Variant i = { .type = TYPE_INT, .data.as_integer = 100 };
  Variant f = { .type = TYPE_FLOAT, .data.as_float = 100.34 };
  Variant s = { .type = TYPE_STRING, .data.as_string = "Yo Dude!" };

  Variant_print(&i);
  Variant_print(&f);
  Variant_print(&s);

  i.data.as_integer = 200;
  f.data.as_float = 2.345;
  s.data.as_string = "Hi there.";

  Variant_print(&i);
  Variant_print(&f);
  Variant_print(&s);

  return 0;
}