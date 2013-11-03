#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bytecodes/x264.h"
#include "rvex.h"

typedef signed char        int8_t;
typedef unsigned char      uint8_t;
typedef signed short       int16_t;
typedef unsigned short     uint16_t;
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

// in: a pseudo-simd number of the form x+(y<<16)
// return: abs(x)+(abs(y)<<16)
static sum2_t abs2( sum2_t a )
{
    sum2_t s = ((a>>(BITS_PER_SUM-1))&(((sum2_t)1<<BITS_PER_SUM)+1))*((sum_t)-1);
    return (a+s)^s;
}

int x264_pixel_satd_8x4( pixel *pix1, intptr_t i_pix1, pixel *pix2, intptr_t i_pix2 )
{
    sum2_t tmp[4][4];
    sum2_t a0, a1, a2, a3;
    sum2_t sum = 0;
    int i;
    for( i = 0; i < 4; i++, pix1 += i_pix1, pix2 += i_pix2 )
    {
        a0 = (pix1[0] - pix2[0]) + ((sum2_t)(pix1[4] - pix2[4]) << BITS_PER_SUM);
        a1 = (pix1[1] - pix2[1]) + ((sum2_t)(pix1[5] - pix2[5]) << BITS_PER_SUM);
        a2 = (pix1[2] - pix2[2]) + ((sum2_t)(pix1[6] - pix2[6]) << BITS_PER_SUM);
        a3 = (pix1[3] - pix2[3]) + ((sum2_t)(pix1[7] - pix2[7]) << BITS_PER_SUM);
        HADAMARD4( tmp[i][0], tmp[i][1], tmp[i][2], tmp[i][3], a0,a1,a2,a3 );
    }
    for( i = 0; i < 4; i++ )
    {
        HADAMARD4( a0, a1, a2, a3, tmp[0][i], tmp[1][i], tmp[2][i], tmp[3][i] );
        sum += abs2(a0) + abs2(a1) + abs2(a2) + abs2(a3);
    }
    return (((sum_t)sum) + (sum>>BITS_PER_SUM)) >> 1;
}

void printCharArray(char numbers[], int length) {
    int i;
    for (i = 0; i < length; i++) {
        printf("%d%c", numbers[i], i == (length - 1) ? '\n' : ',');
    }
}

int vex_pixel_satd_8x4( pixel *pix1, intptr_t i_pix1, pixel *pix2, intptr_t i_pix2 )
{
    unsigned int result;
    char data[64];

    int i = 0;
    int j = 0;
    int k = 0;

    rvexSeek(0);

    for (i = 0; i < 4; i++, pix1 += i_pix1) {
        for (j = 0; j < 8; j++) {
            data[k++] = pix1[j];
        }
    }

    for (i = 0; i < 4; i++, pix2 += i_pix2) {
        for (j = 0; j < 8; j++) {
            data[k++] = pix2[j];
        }
    }

    rvexWrite(data, 64);

    rvexGo();

    rvexSeek(0);
    rvexRead(&result, 4);

    return result;
}

void help(char **argv)
{
    printf("Usage: %s <file>\n", argv[0]);
}

int read_pixels(FILE *fr, intptr_t *i_pix1, pixel **pix1, intptr_t *i_pix2, pixel **pix2)
{
    int x, y, j, max;

    fscanf(fr, "%d,%d", &x, &y);

    (*i_pix1) = (intptr_t) x;
    (*i_pix2) = (intptr_t) y;

    max = x > y ? x : y;

    (*pix1) = (pixel*) malloc(4 * max * sizeof(pixel));
    (*pix2) = (pixel*) malloc(4 * max * sizeof(pixel));

    j = 0;

    while (fscanf(fr, "%d,%d", &x, &y) != EOF) {

        (*pix1)[j] = x;
        (*pix2)[j] = y;

        if (++j == 4 * max) {
            return j;
        }
    }

    free(*pix1);
    free(*pix2);

    return 0;
}

int main(int argc, char **argv)
{
    FILE *fr;
    intptr_t i_pix1;
    intptr_t i_pix2;
    pixel *pix1;
    pixel *pix2;
    int j, f;
    int use_vex = 0;

    if (argc < 2) {
        help(argv);
        return 1;
    }

    printf("number of args:%d %s\n", argc, argv[2]);

    if (argc >= 3) {
        if (strcmp(argv[2], "--vex") == 0) {
            use_vex = 1;
            printf("using vex\n");
        }
    }

    fr = fopen(argv[1], "r");

    if (use_vex) {
        rvexInit(bytecode, sizeof(bytecode));
    }

    while ((j = read_pixels(fr, &i_pix1, &pix1, &i_pix2, &pix2)) != 0) {

        if (use_vex) {
            f = vex_pixel_satd_8x4(pix1, i_pix1, pix2, i_pix2);
        } else {
            f = x264_pixel_satd_8x4(pix1, i_pix1, pix2, i_pix2);
        }

        printf("satd = %d\n", f);

        free(pix1);
        free(pix2);
    }

    fclose(fr);

    if (use_vex) {
        rvexDeInit();
    }

    printf("hello world %s\n", argv[0]);
    return 0;
}


