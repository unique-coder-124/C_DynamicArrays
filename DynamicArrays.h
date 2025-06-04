#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdlib.h>
#include <stddef.h>

#define init_NamedDynamicArray_ofType(name, type)                                       \
  typedef struct {                                                                      \
    type* items;                                                                        \
    size_t count;                                                                       \
    size_t capacity;                                                                    \
  } name

#define DynamicArray_Append(arr, item) do {                                             \
  if (arr.count > arr.capacity) {                                                       \
    if (arr.capacity == 0) {                                                            \
      arr.capacity = 128;                                                               \
    } else {                                                                            \
      arr.capacity *= 2;                                                                \
    }                                                                                   \
    arr.items = realloc(arr.items, arr.capacity*sizeof(*arr.items));                    \
                                                                                        \
  }                                                                                     \
  arr.items[arr.count++] = item;                                                        \
} while (0)

#endif
