#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

static inline uint32_t next_pow2(uint32_t x) {
    if (x <= 2) {
        return x;
    } else {
        return (1U << 31) >> (__builtin_clz(x - 1)-1);
    }
}

#endif // UTILS_H