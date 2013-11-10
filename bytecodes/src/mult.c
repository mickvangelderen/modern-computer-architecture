int a = 0xAAA00A00, b = 0xBBB00B00, result = 0xCCC00C00;

#define FLIP_ENDI_32(a) ( \
		(((a)&0x000000FF) << 24) | \
		(((a)&0x0000FF00) << 8) | \
		(((a)&0x00FF0000) >> 8) | \
		(((a)&0xFF000000) >> 24) \
	)

int main () {
	a = FLIP_ENDI_32(a);
	b = FLIP_ENDI_32(b);
	result = a * b;
	result = FLIP_ENDI_32(result);
	return 0;
}