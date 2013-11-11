
int a[3];

#define FLIP_ENDI_32(a) ( \
        (((a)&0x000000FF) << 24) | \
        (((a)&0x0000FF00) << 8) | \
        (((a)&0x00FF0000) >> 8) | \
        (((a)&0xFF000000) >> 24) \
    )


int main () {
	a[2] = FLIP_ENDI_32(FLIP_ENDI_32(a[0]) + FLIP_ENDI_32(a[1]));
	return 0;
}
