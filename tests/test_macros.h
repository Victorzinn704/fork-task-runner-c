#ifndef FTR_TEST_MACROS_H
#define FTR_TEST_MACROS_H

#include <stdio.h>

#define TEST_ASSERT(condition)                                                                     \
  do {                                                                                             \
    if (!(condition)) {                                                                            \
      fprintf(stderr, "assertion failed: %s:%d: %s\n", __FILE__, __LINE__, #condition);            \
      return 1;                                                                                    \
    }                                                                                              \
  } while (0)

#define RUN_TEST(test_function)                                                                    \
  do {                                                                                             \
    int result = test_function();                                                                  \
    if (result != 0) {                                                                             \
      fprintf(stderr, "test failed: %s\n", #test_function);                                        \
      return result;                                                                               \
    }                                                                                              \
  } while (0)

#endif
