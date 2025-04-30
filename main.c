#include <stdio.h>
#include "DynamicArrays.h"

init_NamedDynamicArray_ofType(IntArray, int);

int main(int argc, char **argv) {
  IntArray test;

  for (int i = 0; i < 100; i++) {
    DynamicArray_Append(test, i);
  }

  for (int i = 0; i < test.count; i++) {
    printf("%d\n", test.items[i]);
  }

  free(test.items);
  return 0;
}
