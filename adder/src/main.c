#include <stdio.h>
#include <stdlib.h>

#include "rovex.h"
#include "bytecodes/add.h"

int main(int argc, char **argv) {

	const unsigned char RVEX_CLEAR = '2';
	const unsigned char RVEX_START = '1';

	unsigned char status;

	if (argc != 3) {
		printf("Use like this: `%s a b` where a and b are integers\n", argv[0]);
		return -1;
	}

	int a = atoi(argv[1]);
	int b = atoi(argv[2]);
	int result;
	a = changeEndianess32(a);
	b = changeEndianess32(b);

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

	write(fd_mem, &a, sizeof(int));
	write(fd_mem, &b, sizeof(int));
	printf("Wrote 0x%08X offset 0 to \"%s\"\n", a, RVEX_DATA_MEMORY_FILE);
	printf("Wrote 0x%08X offset 4 to \"%s\"\n", b, RVEX_DATA_MEMORY_FILE);

	// Clear status and start procedure
	write(fd_ctl, &RVEX_CLEAR, sizeof(unsigned char));
	write(fd_ctl, &RVEX_START, sizeof(unsigned char));
	printf("Wrote clear and start to \"%s\"\n", RVEX_CORE_CTL_FILE);

	do {
		read(fd_stat, &status, sizeof(unsigned char));
		printf("Status %d", status);
	} while (status != '3');

	lseek(fd_mem, 2*sizeof(int), SEEK_SET);
	read(fd_mem, &result, sizeof(int));

	close(fd_mem);
	close(fd_ctl);
	close(fd_stat);
	close(fd_instr);

	printf("Operation executed\n");
	printf("Read 0x%08X offset 8 from \"%s\"\n", result, RVEX_DATA_MEMORY_FILE);
	printf("The result of %d + %d is %d\n", changeEndianess32(a), changeEndianess32(b), changeEndianess32(result));

	return 0;
}
