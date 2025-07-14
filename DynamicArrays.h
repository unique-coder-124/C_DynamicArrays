#include <stdlib.h>
#include <stddef.h>

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

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

#define DA_Append(_ARR, _VAL) ({                                              \
  DIAGNOSTIC_PUSH                                                             \
  DIAGNOSTIC_IGNORE_UNUSED                                                    \
  int _RESULT = 1;                                                            \
  if (_ARR.count >= _ARR.capacity) {                                          \
    if (_ARR.capacity == 0) {                                                 \
      _ARR.capacity = 128;                                                    \
    } else {                                                                  \
      _ARR.capacity *= 2;                                                     \
    }                                                                         \
    void* _OLDPTR = _ARR.items;                                               \
    _ARR.items = realloc(_ARR.items, _ARR.capacity*sizeof(*_ARR.items));      \
    if (!_ARR.items) {                                                        \
      _RESULT = 0;                                                            \
      _ARR.items = _OLDPTR;                                                   \
    } else {                                                                  \
      _ARR.items[_ARR.count++] = _VAL;                                        \
    }                                                                         \
  } else {                                                                    \
    _ARR.items[_ARR.count++] = _VAL;                                          \
  }                                                                           \
  _RESULT;                                                                    \
  DIAGNOSTIC_POP                                                              \
})

#define DA_Remove(_ARR, _IDX) ({                          \
  DIAGNOSTIC_PUSH                                         \
  DIAGNOSTIC_IGNORE_UNUSED                                \
  int _RESULT = 1;                                        \
  if (_IDX < 0 || _IDX >= _ARR.count) {                   \
    _RESULT = 0;                                          \
  } else {                                                \
    for (int _I = _IDX; _I < _ARR.count - 1; _I++) {      \
      _ARR.items[_I] = _ARR.items[_I + 1];                \
    }                                                     \
    _ARR.count--;                                         \
  }                                                       \
  _RESULT;                                                \
  DIAGNOSTIC_POP                                          \
})

#define DA_Insert(_ARR, _IDX, _VAL) ({                                        \
  DIAGNOSTIC_PUSH                                                             \
  DIAGNOSTIC_IGNORE_UNUSED                                                    \
  int _RESULT = 1;                                                            \
  if (_ARR.count >= _ARR.capacity) {                                          \
    if (_ARR.capacity == 0) {                                                 \
      _ARR.capacity = 128;                                                    \
    } else {                                                                  \
      _ARR.capacity *= 2;                                                     \
    }                                                                         \
    void* _OLDPTR = _ARR.items;                                               \
    _ARR.items = realloc(_ARR.items, _ARR.capacity*sizeof(*_ARR.items));      \
    if (!_ARR.items) {                                                        \
      _RESULT = 0;                                                            \
      _ARR.items = _OLDPTR;                                                   \
    } else {                                                                  \
      _ARR.count++;                                                           \
      for (int _I = _ARR.count; _I > _IDX; _I--) {                            \
        _ARR.items[_I] = _ARR.items[_I - 1];                                  \
      }                                                                       \
      _ARR.items[_IDX] = _VAL;                                                \
    }                                                                         \
  } else {                                                                    \
    _ARR.count++;                                                             \
    for (int _I = _ARR.count; _I > _IDX; _I--) {                              \
      _ARR.items[_I] = _ARR.items[_I - 1];                                    \
    }                                                                         \
    _ARR.items[_IDX] = _VAL;                                                  \
  }                                                                           \
  _RESULT;                                                                    \
  DIAGNOSTIC_POP                                                              \
})

#endif
