#include "pixel.h"

pixel pix1[32];
pixel pix2[32];
int result;

#define FLIP_ENDI_32(a) ( \
        (((a)&0x000000FF) << 24) | \
        (((a)&0x0000FF00) << 8) | \
        (((a)&0x00FF0000) >> 8) | \
        (((a)&0xFF000000) >> 24) \
    )

sum2_t abs2(sum2_t a) {
    sum2_t s = ((a>>(BITS_PER_SUM-1))&(((sum2_t)1<<BITS_PER_SUM)+1))*((sum_t)-1);
    return (a+s)^s;
}

int x264_pixel_satd_8x4( pixel *pix1, int i_pix1, pixel *pix2, int i_pix2 )
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

int main () {

    result = x264_pixel_satd_8x4(pix1, 8, pix2, 8);
    result = FLIP_ENDI_32(result);

    return 0;
}
