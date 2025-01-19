#include <stdbool.h>
#include <stdio.h>

extern int add(int a, int b);
extern void print_number(int number);

int add_ptr(int* a, int* b) { return add(*a, *b); }

int main(int argc) {
  printf("start\n");
  print_number(argc);

  int x = add(32, 32);
  int y = (x * 1);

  int a = add_ptr(&x, &y);
  print_number(a);

  for (int i = 10; i > 0; i--) {
    print_number(i);
  }

  bool z;
  if (x == y) {
    z = true;
  } else {
    z = false;
  }

  if (z) {
    printf("x and y is 64\n");
  } else {
    printf("x and y is not 64\n");
  }

  printf("end\n");

  return (10 - 10) * 10;
}
