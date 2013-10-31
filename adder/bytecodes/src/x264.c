#include "pixel.h"

pixel pixels1[32];
pixel pixels2[32];
int result;

sum2_t abs2(sum2_t a) {
    sum2_t s = ((a>>(BITS_PER_SUM-1))&(((sum2_t)1<<BITS_PER_SUM)+1))*((sum_t)-1);
    return (a+s)^s;
}

#define changeEndianess32(d) \
	(((d) & 0xff000000) >> 24) | \
	(((d) & 0x00ff0000) >> 8) | \
	(((d) & 0x0000ff00) << 8) | \
	(((d) & 0x000000ff) << 24)

#define changeEndianess16(d) \
	(((d) & 0xff00) >> 8) | \
	(((d) & 0x00ff) << 8)

int main () {
//    sum2_t tmp[4][4];
//    sum2_t a0, a1, a2, a3;
//    sum2_t sum = 0;
//
    pixel* pix1 = pixels1;
    pixel* pix2 = pixels2;
//
//    int i;
//    pixel ptemp;
//
//    for (i = 0; i < 32; i++) {
//        pix1[i] = changeEndianess16(pix1[i]);
//        pix2[i] = changeEndianess16(pix2[i]);
//    }
//    for (i = 0; i < 4; i++, pix1 += 8, pix2 += 8) {
//        a0 = (pix1[0] - pix2[0]) + ((sum2_t)(pix1[4] - pix2[4]) << BITS_PER_SUM);
//        a1 = (pix1[1] - pix2[1]) + ((sum2_t)(pix1[5] - pix2[5]) << BITS_PER_SUM);
//        a2 = (pix1[2] - pix2[2]) + ((sum2_t)(pix1[6] - pix2[6]) << BITS_PER_SUM);
//        a3 = (pix1[3] - pix2[3]) + ((sum2_t)(pix1[7] - pix2[7]) << BITS_PER_SUM);
//        HADAMARD4(tmp[i][0], tmp[i][1], tmp[i][2], tmp[i][3], a0,a1,a2,a3);
//    }
//    for (i = 0; i < 4; i++) {
//        HADAMARD4(a0, a1, a2, a3, tmp[0][i], tmp[1][i], tmp[2][i], tmp[3][i]);
//        sum += abs2(a0) + abs2(a1) + abs2(a2) + abs2(a3);
//    }
//
//    result = (((sum_t)sum) + (sum>>BITS_PER_SUM)) >> 1;
//    result = changeEndianess32(result);

    result = pixels1[0];

    return 0;
}
