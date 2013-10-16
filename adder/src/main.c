#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

const char * const RVEX_CORE_CTL_FILE = "/sys/devices/virtual/rvex_core/rvex-smemory.0/rvex_core_ctl";
const char * const RVEX_CORE_STATUS_FILE = "/sys/devices/virtual/rvex_core/rvex-smemory.0/rvex_core_status";
const char * const RVEX_DATA_MEMORY_FILE = "/dev/rvex-dmemory.0";
const char * const RVEX_INSTRUCTION_MEMORY_FILE = "/dev/rvex-imemory.0";

// works like open(const char * path, int oflag, ...) but only supports path and flag parameters. Prints errors if any. 
int myopen(const char * path, int oflag) {
	// Buffer for error message
	const size_t BUFFER_SIZE = 256;
	char buffer[BUFFER_SIZE];

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

int main(int argc, char **argv) {
	
	unsigned char RVEX_CLEAR = 2;
	unsigned char RVEX_START = 1;

	unsigned char status = 0;

	char data[] = {0x05,0,0,0,0x03,0,0,0};

	// Write some data to memory
	printf("Opening files...\n");
	int fd_instr = myopen(RVEX_INSTRUCTION_MEMORY_FILE, O_WRONLY);
	int fd_mem = myopen(RVEX_DATA_MEMORY_FILE, O_RDWR);
	int fd_ctl = myopen(RVEX_CORE_CTL_FILE, O_WRONLY);
	int fd_stat = myopen(RVEX_CORE_STATUS_FILE, O_RDONLY);
	FILE * f_bytecode = fopen("bytecode-11", "rb");

	char buffer[64];
	size_t num_read;

	if (fd_instr < 0 || fd_mem < 0 || fd_ctl < 0 || fd_stat < 0 || f_bytecode == NULL) {
		return -1;
	}

	while((num_read = fread(buffer, sizeof(char), 64, f_bytecode)) != 0) {
		write(fd_instr, buffer, num_read);
		printf("Wrote %d bytes to instruction memory\n", (int) num_read);
	}

	write(fd_mem, data, 8);
	printf("Wrote to \"%s\"\n", RVEX_DATA_MEMORY_FILE);

	// Clear status and start procedure
	write(fd_ctl, &RVEX_CLEAR, 1);
	write(fd_ctl, &RVEX_START, 1);
	printf("Wrote clear and start to \"%s\"\n", RVEX_CORE_CTL_FILE);
	
	lseek(fd_mem, 8, SEEK_SET);
	read(fd_mem, &status, 1);
	printf("The result of 0x05 + 0x03 is %d\n", status);

	close(fd_mem);
	close(fd_ctl);
	close(fd_stat);
	close(fd_instr);

	return 0;
}
