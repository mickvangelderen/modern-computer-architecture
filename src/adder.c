#include <stdio.h>
#include <stdlib.h>

#include "bytecodes/adder.h"
#include "rvex.h"

int main(int argc, char **argv) {

	if (argc != 3) {
		printf("Use like this: `%s a b` where a and b are integers\n", argv[0]);
		return -1;
	}

	int a = atoi(argv[1]);
	int b = atoi(argv[2]);
	int c = a + 1;
	int d = b + 2;
	a = changeEndianess32(a);
	b = changeEndianess32(b);
	c = changeEndianess32(c);
	d = changeEndianess32(d);

	int result1, result2;

	rvexInit(bytecode, sizeof(bytecode));

	// fist time
	rvexWrite(&a, sizeof(int));
	rvexWrite(&b, sizeof(int));

	// Clear status and start procedure
	rvexGo();

	rvexSeek(2 * sizeof(int));

	rvexRead(&result1, sizeof(int));

	printf("result of %d + %d = %d\n",
			changeEndianess32(a),
			changeEndianess32(b),
			changeEndianess32(result1));

	// second time

	rvexSeek(0);

	rvexWrite(&c, sizeof(int));
	rvexWrite(&d, sizeof(int));

	// Clear status and start procedure
	rvexGo();

	rvexSeek(2 * sizeof(int));

	rvexRead(&result2, sizeof(int));

	printf("result of %d + %d = %d\n",
			changeEndianess32(c),
			changeEndianess32(d),
			changeEndianess32(result2));

	// done
	rvexDeInit();

	printf("done\n");

	return 0;
}
