#include <stdio.h>
#include <stdlib.h>
#include "rvex.h"
#include "mult.bh"

int main(int argc, char **argv) {

	if (argc != 3) {
		printf("Use like this: `%s n c` to let the program calculate n^1..c\n", argv[0]);
		return -1;
	}

	rvexInit(bytecode, sizeof(bytecode));
	int n = atoi(argv[1]);
	int c = atoi(argv[2]);
	int temp = 1;
	while(c > 0) {
		// printf("in loop c = %d\n\tn = %d\t0x%08X\n\ttemp = %d\t0x%08X\n", c, n, n, temp, temp);
		rvexSeek(0);
		rvexWrite(&n, sizeof(int));
		rvexWrite(&temp, sizeof(int));
		rvexGo();
		rvexRead(&temp, sizeof(int));
		c--;
	}

	printf("n^c = %d\n", temp);

	void rvexDeInit();

	return 0;
}
