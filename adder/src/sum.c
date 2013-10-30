#include <stdio.h>
#include <stdlib.h>

#include "rovex.h"
#include "sum.h"

int printIntArray(int numbers[], int length) {
	int i;
	for (i = 0; i < length; i++) {
		printf("%d%c", numbers[i], i == (length - 1) ? '\n' : ',');
	}
}

int main(int argc, char **argv) {

	const unsigned char RVEX_CLEAR = '2';
	const unsigned char RVEX_START = '1';

	unsigned char status;

	int * numbers;
	int l = argc - 1;
	int i;

	numbers = (int*) malloc(l * sizeof(int));

	for (i = 1; i < argc; i++) {
		numbers[i - 1] = atoi(argv[i]);
	}

	printf("using these numbers\n");
	printIntArray(numbers, l);

	for (i = 0; i < l; i++) {
		numbers[i] = changeEndianess32(numbers[i]);
	}

	printf("using these numbers, changed endianess\n");
	printIntArray(numbers, l);

	printf("%d %d\n", (int) sizeof(int), (int) sizeof(char));

	int result;

	// Write some data to memory
	printf("Opening files...\n");
	//FILE * f_bytecode = myfopen("bytecode-11", "rb");
	int fd_instr = myopen(RVEX_INSTRUCTION_MEMORY_FILE, O_WRONLY);
	int fd_mem = myopen(RVEX_DATA_MEMORY_FILE, O_RDWR);
	int fd_ctl = myopen(RVEX_CORE_CTL_FILE, O_WRONLY);
	int fd_stat = myopen(RVEX_CORE_STATUS_FILE, O_RDONLY);

	if (fd_instr < 0 || fd_mem < 0 || fd_ctl < 0 || fd_stat < 0) {
		return -1;
	}

	// Incorporated the bytecode into the executable ;D
	write(fd_instr, bytecode, sizeof(bytecode));

	// write data
	write(fd_mem, numbers, l * sizeof(int));
	printf("Wrote this that to \"%s\"\n", RVEX_DATA_MEMORY_FILE);
	printIntArray(numbers, l);

	// Clear status and start procedure
	write(fd_ctl, &RVEX_CLEAR, sizeof(unsigned char));
	write(fd_ctl, &RVEX_START, sizeof(unsigned char));
	printf("Wrote clear and start to \"%s\"\n", RVEX_CORE_CTL_FILE);

	do {
		read(fd_stat, &status, sizeof(unsigned char));
		printf("Status %d", status);
	} while (status != '3');

	lseek(fd_mem, 2 * sizeof(int), SEEK_SET);
	read(fd_mem, &result, sizeof(int));

	close(fd_mem);
	close(fd_ctl);
	close(fd_stat);
	close(fd_instr);

	printf("Operation executed\n");
	printf("Read 0x%08X offset 8 from \"%s\"\n", result, RVEX_DATA_MEMORY_FILE);
	printf("The result is %d\n", changeEndianess32(result));

	return 0;
}
