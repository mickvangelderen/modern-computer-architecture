
#include <stdio.h>

//static NOINLINE int x264_pixel_satd_8x4( pixel *pix1, intptr_t i_pix1, pixel *pix2, intptr_t i_pix2 )
//{
//    sum2_t tmp[4][4];
//    sum2_t a0, a1, a2, a3;
//    sum2_t sum = 0;
//    for( int i = 0; i < 4; i++, pix1 += i_pix1, pix2 += i_pix2 )
//    {
//        a0 = (pix1[0] - pix2[0]) + ((sum2_t)(pix1[4] - pix2[4]) << BITS_PER_SUM);
//        a1 = (pix1[1] - pix2[1]) + ((sum2_t)(pix1[5] - pix2[5]) << BITS_PER_SUM);
//        a2 = (pix1[2] - pix2[2]) + ((sum2_t)(pix1[6] - pix2[6]) << BITS_PER_SUM);
//        a3 = (pix1[3] - pix2[3]) + ((sum2_t)(pix1[7] - pix2[7]) << BITS_PER_SUM);
//        HADAMARD4( tmp[i][0], tmp[i][1], tmp[i][2], tmp[i][3], a0,a1,a2,a3 );
//    }
//    for( int i = 0; i < 4; i++ )
//    {
//        HADAMARD4( a0, a1, a2, a3, tmp[0][i], tmp[1][i], tmp[2][i], tmp[3][i] );
//        sum += abs2(a0) + abs2(a1) + abs2(a2) + abs2(a3);
//    }
//    return (((sum_t)sum) + (sum>>BITS_PER_SUM)) >> 1;
//}

int main(char * argv)
{
	printf("hello world\n");
	return 0;
}
