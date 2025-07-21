#include <stdlib.h>
#include <stddef.h>

#ifndef DYNAMIC__DA_ARRAY_H
#define DYNAMIC__DA_ARRAY_H

#if defined(__clang__)
  #define DIAGNOSTIC_PUSH    _Pragma("clang diagnostic push")
  #define DIAGNOSTIC_IGNORE_FORMAT  _Pragma("clang diagnostic ignored \"-Wformat\"")
  #define DIAGNOSTIC_IGNORE_UNUSED  _Pragma("clang diagnostic ignored \"-Wunused-value\"")
  #define DIAGNOSTIC_POP     _Pragma("clang diagnostic pop")
#elif defined(__GNUC__)
  #define DIAGNOSTIC_PUSH    _Pragma("GCC diagnostic push")
  #define DIAGNOSTIC_IGNORE_FORMAT  _Pragma("GCC diagnostic ignored \"-Wformat\"")
  #define DIAGNOSTIC_IGNORE_UNUSED  _Pragma("GCC diagnostic ignored \"-Wunused-value\"")
  #define DIAGNOSTIC_POP     _Pragma("GCC diagnostic pop")
#else
  #define DIAGNOSTIC_PUSH
  #define DIAGNOSTIC_IGNORE_FORMAT
  #define DIAGNOSTIC_IGNORE_UNUSED
  #define DIAGNOSTIC_POP
#endif

#define DA(type)          \
  struct {                \
    type* items;          \
    size_t count;         \
    size_t capacity;      \
  }

#define DA_Append(__DA_ARR, __DA_VAL) ({                                                              \
  DIAGNOSTIC_PUSH                                                                                     \
  DIAGNOSTIC_IGNORE_UNUSED                                                                            \
  int __DA_RESULT = 1;                                                                                \
  if ((__DA_ARR).count >= (__DA_ARR).capacity) {                                                      \
    if ((__DA_ARR).capacity == 0) {                                                                   \
      (__DA_ARR).capacity = 128;                                                                      \
    } else {                                                                                          \
      (__DA_ARR).capacity *= 2;                                                                       \
    }                                                                                                 \
    void* __DA_OLDPTR = (__DA_ARR).items;                                                             \
    (__DA_ARR).items = realloc((__DA_ARR).items, (__DA_ARR).capacity*sizeof(*(__DA_ARR).items));      \
    if (!(__DA_ARR).items) {                                                                          \
      __DA_RESULT = 0;                                                                                \
      (__DA_ARR).items = __DA_OLDPTR;                                                                 \
    } else {                                                                                          \
      (__DA_ARR).items[(__DA_ARR).count++] = __DA_VAL;                                                \
    }                                                                                                 \
  } else {                                                                                            \
    (__DA_ARR).items[(__DA_ARR).count++] = __DA_VAL;                                                  \
  }                                                                                                   \
  __DA_RESULT;                                                                                        \
  DIAGNOSTIC_POP                                                                                      \
})

#define DA_Remove(__DA_ARR, __DA_IDX) ({                                                     \
  DIAGNOSTIC_PUSH                                                                            \
  DIAGNOSTIC_IGNORE_UNUSED                                                                   \
  int __DA_RESULT = 1;                                                                       \
  if (((size_t)__DA_IDX) < 0 || __DA_IDX >= (__DA_ARR).count) {                              \
    __DA_RESULT = 0;                                                                         \
  } else {                                                                                   \
    for (size_t __DA_I = ((size_t)__DA_IDX); __DA_I < (__DA_ARR).count - 1; __DA_I++) {      \
      (__DA_ARR).items[__DA_I] = (__DA_ARR).items[__DA_I + 1];                               \
    }                                                                                        \
    (__DA_ARR).count--;                                                                      \
  }                                                                                          \
  __DA_RESULT;                                                                               \
  DIAGNOSTIC_POP                                                                             \
})

#define DA_Insert(__DA_ARR, __DA_IDX, __DA_VAL) ({                                                    \
  DIAGNOSTIC_PUSH                                                                                     \
  DIAGNOSTIC_IGNORE_UNUSED                                                                            \
  int __DA_RESULT = 1;                                                                                \
  if ((__DA_ARR).count >= (__DA_ARR).capacity) {                                                      \
    if ((__DA_ARR).capacity == 0) {                                                                   \
      (__DA_ARR).capacity = 128;                                                                      \
    } else {                                                                                          \
      (__DA_ARR).capacity *= 2;                                                                       \
    }                                                                                                 \
    void* __DA_OLDPTR = (__DA_ARR).items;                                                             \
    (__DA_ARR).items = realloc((__DA_ARR).items, (__DA_ARR).capacity*sizeof(*(__DA_ARR).items));      \
    if (!(__DA_ARR).items) {                                                                          \
      __DA_RESULT = 0;                                                                                \
      (__DA_ARR).items = __DA_OLDPTR;                                                                 \
    } else {                                                                                          \
      (__DA_ARR).count++;                                                                             \
      for (size_t __DA_I = (__DA_ARR).count; __DA_I > ((size_t)__DA_IDX); __DA_I--) {                 \
        (__DA_ARR).items[__DA_I] = (__DA_ARR).items[__DA_I - 1];                                      \
      }                                                                                               \
      (__DA_ARR).items[((size_t)__DA_IDX)] = __DA_VAL;                                                \
    }                                                                                                 \
  } else {                                                                                            \
    (__DA_ARR).count++;                                                                               \
    for (size_t __DA_I = (__DA_ARR).count; __DA_I > ((size_t)__DA_IDX); __DA_I--) {                   \
      (__DA_ARR).items[__DA_I] = (__DA_ARR).items[__DA_I - 1];                                        \
    }                                                                                                 \
    (__DA_ARR).items[((size_t)__DA_IDX)] = __DA_VAL;                                                  \
  }                                                                                                   \
  __DA_RESULT;                                                                                        \
  DIAGNOSTIC_POP                                                                                      \
})

#define DA_ResizeToFit(__DA_ARR) ({                                                              \
  DIAGNOSTIC_PUSH                                                                                \
  DIAGNOSTIC_IGNORE_UNUSED                                                                       \
  int __DA_RESULT = 1;                                                                           \
  void* __DA_OLDPTR = (__DA_ARR).items;                                                          \
  (__DA_ARR).items = realloc((__DA_ARR).items, (__DA_ARR).count*sizeof(*(__DA_ARR).items));      \
  if (!(__DA_ARR).items) {                                                                       \
    __DA_RESULT = 0;                                                                             \
    (__DA_ARR).items = __DA_OLDPTR;                                                              \
  } else {                                                                                       \
    (__DA_ARR).capacity = (__DA_ARR).count;                                                      \
  }                                                                                              \
  __DA_RESULT;                                                                                   \
  DIAGNOSTIC_POP                                                                                 \
})

#define DA_Resize(__DA_ARR, __DA_SIZE) ({                                                 \
  DIAGNOSTIC_PUSH                                                                         \
  DIAGNOSTIC_IGNORE_UNUSED                                                                \
  int __DA_RESULT = 1;                                                                    \
  void* __DA_OLDPTR = (__DA_ARR).items;                                                   \
  (__DA_ARR).items = realloc((__DA_ARR).items, __DA_SIZE*sizeof(*(__DA_ARR).items));      \
  if (!(__DA_ARR).items) {                                                                \
    __DA_RESULT = 0;                                                                      \
    (__DA_ARR).items = __DA_OLDPTR;                                                       \
  } else {                                                                                \
    (__DA_ARR).capacity = __DA_SIZE;                                                      \
    if (__DA_SIZE < (__DA_ARR).count) {                                                   \
      (__DA_ARR).count = __DA_SIZE;                                                       \
    }                                                                                     \
  }                                                                                       \
  __DA_RESULT;                                                                            \
  DIAGNOSTIC_POP                                                                          \
})

#define DA_Clear(__DA_ARR) ({      \
  DIAGNOSTIC_PUSH                  \
  DIAGNOSTIC_IGNORE_UNUSED         \
  int __DA_RESULT = 1;             \
  (__DA_ARR).count = 0;            \
  __DA_RESULT;                     \
  DIAGNOSTIC_POP                   \
})

#define DA_Zero(__DA_ARR) ({                                          \
  DIAGNOSTIC_PUSH                                                     \
  DIAGNOSTIC_IGNORE_UNUSED                                            \
  int __DA_RESULT = 1;                                                \
  for (size_t __DA_I = 0; __DA_I < (__DA_ARR).count; __DA_I++) {      \
    (__DA_ARR).items[__DA_I] = 0;                                     \
  }                                                                   \
  __DA_RESULT;                                                        \
  DIAGNOSTIC_POP                                                      \
})

#define DA_Free(__DA_ARR) ({          \
  DIAGNOSTIC_PUSH                     \
  DIAGNOSTIC_IGNORE_UNUSED            \
  int __DA_RESULT = 1;                \
  if ((__DA_ARR).capacity > 0) {      \
    free((__DA_ARR).items);           \
    (__DA_ARR).count = 0;             \
    (__DA_ARR).capacity = 0;          \
  }                                   \
  __DA_RESULT;                        \
  DIAGNOSTIC_POP                      \
})

#endif
