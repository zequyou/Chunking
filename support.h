//
// Created by developer on 11/1/19.
//

#ifndef CHUNKING_SUPPORT_H
#define CHUNKING_SUPPORT_H

#include <cstdint>
#include <cmath>

#define POLY_MASK (0xffffffffffULL)
#define	FP_POLY  0xbfe6b8a5bf378d83ULL
#define	RAB_POLYNOMIAL_CONST 153191
#define RAB_POLYNOMIAL_WIN_SIZE 16
const int CHUNK_SIZE_MAX = 8 * 1024;
const int CHUNK_SIZE_MIN = 4 * 1024 - 512;

#define	RAB_BLK_MIN_BITS 10u
#define	RAB_WINDOW_SLIDE_OFFSET	(64)
#define	RAB_BLK_MASK (((1u << RAB_BLK_MIN_BITS) - 1u) >> 1u)

void create_tables(uint64_t out[256], uint64_t ir[256]) {
    unsigned int term, pow, j;
    uint64_t val, poly_pow;

    poly_pow = 1;
    for (j = 0; j < RAB_POLYNOMIAL_WIN_SIZE; j++) {
        poly_pow = (poly_pow * RAB_POLYNOMIAL_CONST) & POLY_MASK;
    }

    for (j = 0; j < 256; j++) {
        term = 1;
        pow = 1;
        val = 1;
        out[j] = (j * poly_pow) & POLY_MASK;
        for (int i = 0; i < RAB_POLYNOMIAL_WIN_SIZE; i++) {
            if (term & FP_POLY) {
                val += ((pow * j) & POLY_MASK);
            }
            pow = (pow * RAB_POLYNOMIAL_CONST) & POLY_MASK;
            term <<= 1u;
        }
        ir[j] = val;
    }
}

uint32_t get_min_block_size() {
    uint32_t min_block_size;

    min_block_size = (1u << (2 + RAB_BLK_MIN_BITS)) - 1024 / (uint32_t)pow(2, 11 - RAB_BLK_MIN_BITS);
    return min_block_size;
}


#endif //CHUNKING_SUPPORT_H
