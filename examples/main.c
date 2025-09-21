#include "stdio.h"

int main(int argc, char* argv[]) {
  char* program = argv[argc - 1];
  printf("%s\n", program);

  return 0;
}
