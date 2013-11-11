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

	int result1, result2;

	rvexInit(&rvex0, bytecode, sizeof(bytecode),
		RVEX_0_INSTRUCTION_MEMORY_FILE,
		RVEX_0_DATA_MEMORY_FILE,
		RVEX_0_CORE_CTL_FILE,
		RVEX_0_CORE_STATUS_FILE);

	// fist time
	rvexWrite(&rvex0, &a, sizeof(int));
	rvexWrite(&rvex0, &b, sizeof(int));

	// Clear status and start procedure
	rvexGo(&rvex0);

	rvexSeek(&rvex0, 2 * sizeof(int));

	rvexRead(&rvex0, &result1, sizeof(int));

	printf("result of %d + %d = %d\n",
			a,
			b,
			result1);

	// second time

	rvexSeek(&rvex0, 0);

	rvexWrite(&rvex0, &c, sizeof(int));
	rvexWrite(&rvex0, &d, sizeof(int));

	// Clear status and start procedure
	rvexGo(&rvex0);

	rvexSeek(&rvex0, 2 * sizeof(int));

	rvexRead(&rvex0, &result2, sizeof(int));

	printf("result of %d + %d = %d\n",
			c,
			d,
			result2);

	// done
	rvexDispose(&rvex0);

	printf("done\n");

	return 0;
}
