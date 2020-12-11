#ifndef TEST_UITL_H
#define TEST_UTIL_H

#include <cstdio>

#define EXPECT(cond)                                                            \
    if (!(cond)) {                                                              \
        fprintf(stderr, "%s: line %d: %s failed\n", __FILE__, __LINE__, #cond); \
    }

#define ASSERT(cond)                                                            \
    if (!(cond)) {                                                              \
        fprintf(stderr, "%s: line %d: %s failed\n", __FILE__, __LINE__, #cond); \
        abort();                                                                \
    }

#endif