#ifndef MCA_PIXEL_H
#define MCA_PIXEL_H

typedef signed char        int8_t;
typedef unsigned char      uint8_t;
typedef signed short       int16_t;
typedef unsigned short     uint16_t;
typedef signed long int    int32_t;
typedef unsigned long int  uint32_t;

#define BIT_DEPTH 8

#if BIT_DEPTH > 8
    typedef uint32_t sum_t;
    typedef uint64_t sum2_t;
    typedef uint16_t pixel;
#else
    typedef uint16_t sum_t;
    typedef uint32_t sum2_t;
    typedef uint8_t  pixel;
#endif

#define BITS_PER_SUM (8 * sizeof(sum_t))

#define HADAMARD4(d0, d1, d2, d3, s0, s1, s2, s3) {\
    sum2_t t0 = s0 + s1;\
    sum2_t t1 = s0 - s1;\
    sum2_t t2 = s2 + s3;\
    sum2_t t3 = s2 - s3;\
    d0 = t0 + t2;\
    d2 = t0 - t2;\
    d1 = t1 + t3;\
    d3 = t1 - t3;\
}

#endif
