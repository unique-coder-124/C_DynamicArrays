#include <stdio.h>
#include "DynamicArrays.h"

typedef DA(int) IntArray;

int main(int argc, char **argv) {
  IntArray test = {0};

  for (int i = 0; i < 100; i++) {
    DA_Append(test, i);
  }

  printf("%zd\n", test.count);

  printf("[");
  for (int i = 0; i < test.count - 1; i++) {
    printf("%d, ", test.items[i]);
  }
  printf("%d]", test.items[test.count - 1]);
  printf("\n");

  for (int i = 0; i < 50; i++) {
    DA_Remove(test, i);
  }

  printf("%zd\n", test.count);

  printf("[");
  for (int i = 0; i < test.count - 1; i++) {
    printf("%d, ", test.items[i]);
  }
  printf("%d]", test.items[test.count - 1]);
  printf("\n");

  for (int i = 0; i < 50; i++) {
    DA_Insert(test, 2*i, 2*i);
  }

  printf("%zd\n", test.count);

  printf("[");
  for (int i = 0; i < test.count - 1; i++) {
    printf("%d, ", test.items[i]);
  }
  printf("%d]", test.items[test.count - 1]);
  printf("\n");

  free(test.items);
  return 0;
}
