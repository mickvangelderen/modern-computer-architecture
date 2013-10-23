#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

const char * const RVEX_CORE_CTL_FILE = "/sys/devices/virtual/rvex_core/rvex-smemory.0/rvex_core_ctl";
const char * const RVEX_CORE_STATUS_FILE = "/sys/devices/virtual/rvex_core/rvex-smemory.0/rvex_core_status";
const char * const RVEX_DATA_MEMORY_FILE = "/dev/rvex-dmemory.0";
const char * const RVEX_INSTRUCTION_MEMORY_FILE = "/dev/rvex-imemory.0";

const size_t BUFFER_SIZE = 1024;
char buffer[1024];

// generated with ../bin2c/bin/bin2c
char bytecode[] = {
	0x00,0x00,0x82,0xC5,0x20,0x00,0x00,0xC2,0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0xC0,
	0x40,0x00,0x00,0x44,0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0xC0,
	0x00,0x00,0x00,0x50,0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0xC0,
	0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0xC0,
	0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0xC0,0x40,0x02,0x84,0x21,0x00,0x00,0x00,0xC2,
	0x00,0x00,0x06,0xC4,0x00,0x00,0x00,0xC0,0x48,0x02,0x88,0x21,0x00,0x00,0x00,0xC2,
	0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0xC0,
	0x80,0x10,0x04,0xC4,0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0xC0,
	0x00,0x00,0x00,0x4C,0x00,0x00,0x00,0xC0,0x50,0x02,0x84,0x2B,0x00,0x00,0x00,0xC2
};

// works like open(const char * path, int oflag, ...) but only supports path and flag parameters. Prints errors if any.
int myopen(const char * path, int oflag) {
	// Ints to save errno and file descriptor
	int e, fd;

	fd = open(path, oflag);
	if (fd < 0) {
		e = errno;
		// Try to convert error code to string and place it in the buffer
		if (strerror_r(e, buffer, BUFFER_SIZE) == 0) {
			printf("Error while opening \"%s\": %s\n", path, buffer);
		} else {
			printf("Error while opening \"%s\": errno %d\n", path, e);
		}
		return -1;
	}
	return fd;
}

FILE * myfopen(const char * path, const char * mode) {
	// Ints to save errno and file descriptor
	int e;
	FILE * f;

	f = fopen(path, mode);
	if (f == NULL) {
		e = errno;
		// Try to convert error code to string and place it in the buffer
		if (strerror_r(e, buffer, BUFFER_SIZE) == 0) {
			printf("Error while opening \"%s\": %s\n", path, buffer);
		} else {
			printf("Error while opening \"%s\": errno %d\n", path, e);
		}
		return NULL;
	}
	return f;
}

#define changeEndianess32(d) \
	(((d) & 0xff000000) >> 24) | \
	(((d) & 0x00ff0000) >> 8) | \
	(((d) & 0x0000ff00) << 8) | \
	(((d) & 0x000000ff) << 24)

#define changeEndianess16(d) \
	(((d) & 0xff00) >> 8) | \
	(((d) & 0x00ff) << 8)

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

	// size_t num_read;

	if (fd_instr < 0 || fd_mem < 0 || fd_ctl < 0 || fd_stat < 0) { // || f_bytecode == NULL) {
		return -1;
	}

	// Copy the instructions to RVEX_INSTRUCTION_MEMORY_FILE
	/*while((num_read = fread(buffer, sizeof(char), BUFFER_SIZE, f_bytecode)) != 0) {
		write(fd_instr, buffer, num_read);
		printf("Wrote %d bytes to instruction memory\n", (int) num_read);
	}
	fclose(f_bytecode);*/

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
