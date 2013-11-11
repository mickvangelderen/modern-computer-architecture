#include <stdio.h>
#include <stdlib.h>
#include "rvex.h"
#include "bytecodes/mult.h"

int main(int argc, char **argv) {

	if (argc != 3) {
		printf("Use like this: `%s n c` to let the program calculate n^1..c\n", argv[0]);
		return -1;
	}

	rvexInit(&rvex0, bytecode, sizeof(bytecode),
		RVEX_0_INSTRUCTION_MEMORY_FILE,
		RVEX_0_DATA_MEMORY_FILE,
		RVEX_0_CORE_CTL_FILE,
		RVEX_0_CORE_STATUS_FILE);

	int n = atoi(argv[1]);
	int c = atoi(argv[2]);
	int temp = 1;
	while(c > 0) {
		// printf("in loop c = %d\n\tn = %d\t0x%08X\n\ttemp = %d\t0x%08X\n", c, n, n, temp, temp);
		rvexSeek(&rvex0, 0);
		rvexWrite(&rvex0, &n, sizeof(int));
		rvexWrite(&rvex0, &temp, sizeof(int));
		rvexGo(&rvex0);
		rvexRead(&rvex0, &temp, sizeof(int));
		c--;
	}

	printf("n^c = %d\n", temp);

	rvexDispose(&rvex0);

	return 0;
}
