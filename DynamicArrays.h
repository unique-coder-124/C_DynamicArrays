#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdlib.h>
#include <stddef.h>

#define DynamicArray(type)      \
  struct {                      \
    type* items;                \
    size_t count;               \
    size_t capacity;            \
  }

#define DynamicArray_Append(_ARR, _ITEM) ({                                 \
  if (_ARR.count >= _ARR.capacity) {                                          \
    if (_ARR.capacity == 0) {                                                 \
      _ARR.capacity = 128;                                                    \
    } else {                                                                  \
      _ARR.capacity *= 2;                                                     \
    }                                                                         \
    _ARR.items = realloc(_ARR.items, _ARR.capacity*sizeof(*_ARR.items));      \
  }                                                                           \
  _ARR.items[_ARR.count++] = _ITEM;                                           \
})

#define DynamicArray_remove(_ARR, _IDX)

#endif
