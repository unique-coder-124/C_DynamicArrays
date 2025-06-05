#include <stdio.h>
#include "DynamicArrays.h"

typedef DynamicArray(int) IntArray;

int main(int argc, char **argv) {
  IntArray test = {0};

  for (int i = 0; i < 100; i++) {
    DynamicArray_Append(test, i);
  }

  printf("%zd\n", test.count);

  printf("[");
  for (int i = 0; i < test.count - 1; i++) {
    printf("%d, ", test.items[i]);
  }
  printf("%d]", test.items[test.count - 1]);

  // free(test.items);
  return 0;
}
