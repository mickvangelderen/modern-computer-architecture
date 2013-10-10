
#include <stdio.h>
#include "pixel.h"

static NOINLINE int x264_pixel_satd_8x4( pixel *pix1, intptr_t i_pix1, pixel *pix2, intptr_t i_pix2 )
{
    sum2_t tmp[4][4];
    sum2_t a0, a1, a2, a3;
    sum2_t sum = 0;
    for( int i = 0; i < 4; i++, pix1 += i_pix1, pix2 += i_pix2 )
    {
        a0 = (pix1[0] - pix2[0]) + ((sum2_t)(pix1[4] - pix2[4]) << BITS_PER_SUM);
        a1 = (pix1[1] - pix2[1]) + ((sum2_t)(pix1[5] - pix2[5]) << BITS_PER_SUM);
        a2 = (pix1[2] - pix2[2]) + ((sum2_t)(pix1[6] - pix2[6]) << BITS_PER_SUM);
        a3 = (pix1[3] - pix2[3]) + ((sum2_t)(pix1[7] - pix2[7]) << BITS_PER_SUM);
        HADAMARD4( tmp[i][0], tmp[i][1], tmp[i][2], tmp[i][3], a0,a1,a2,a3 );
    }
    for( int i = 0; i < 4; i++ )
    {
        HADAMARD4( a0, a1, a2, a3, tmp[0][i], tmp[1][i], tmp[2][i], tmp[3][i] );
        sum += abs2(a0) + abs2(a1) + abs2(a2) + abs2(a3);
    }
    return (((sum_t)sum) + (sum>>BITS_PER_SUM)) >> 1;
}

void help(char **argv)
{
    printf("Usage: %s <file>\n", argv[0]);
}

int read_pixels(FILE *fr, pixel *pix1, pixel *pix2, int max)
{
    int w, v, x, y;

    char buffer[20];
    char c;
    int i;
    int j = 0;

    while ((c = getc(fr)) != EOF) {
        if (c == '\n') {

            buffer[i] = '\0';

            if (i > 1) { // skip empty lines

                sscanf(buffer, "%d,%d", &x, &y);

                pix1[j] = x;
                pix2[j] = y;

                // stop after 4*8 values
                if (++j == 4 * max) {
                    return j;
                }

            }

            i = 0;

        } else if (i < 20) {
            buffer[i++] = c;
        }
    }

    return 0;
}

int main(int argc, char **argv)
{

    if (argc < 2) {
        help(argv);
        return 1;
    }

    FILE *fr = fopen(argv[1], "r");

    intptr_t i_pix1;
    intptr_t i_pix2;
    int x, y, j, i;

    fscanf(fr, "%d,%d", &x, &y);

    i_pix1 = (int) x;
    i_pix2 = (int) y;
    int max = y > x ? y : x;

    pixel *pix1 = (pixel*) malloc(4 * max * sizeof(pixel));
    pixel *pix2 = (pixel*) malloc(4 * max * sizeof(pixel));

    while ((j = read_pixels(fr, pix1, pix2, max)) != 0) {
        for (i = 0; i < max * 4; i++) {
            printf("%d %d\n", pix1[i], pix2[i]);
        }
    }

    int f = x264_pixel_satd_8x4(pix1, i_pix1, pix2, i_pix2);
    printf("satd = %d\n", f);

    fclose(fr);

    printf("hello world %s\n", argv[0]);
    return 0;
}
