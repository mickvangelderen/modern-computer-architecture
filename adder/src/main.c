#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

const char * const RVEX_CORE_CTL_FILE = "/sys/devices/virtual/rvex_core/rvex-smemory.0/rvex_core_ctl";
const char * const RVEX_CORE_STATUS_FILE = "/sys/devices/virtual/rvex_core/rvex-smemory.0/rvex_core_status";

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

	int fd;
	unsigned char status = 0;

	fd = myopen(RVEX_CORE_CTL_FILE, O_WRONLY);
	if (fd < 0) { return -1; }

	write(fd, &RVEX_CLEAR, 1);
	close(fd);
	printf("Wrote clear to \"%s\"", RVEX_CORE_CTL_FILE);

	fd = myopen(RVEX_CORE_STATUS_FILE, O_RDONLY);
	if (fd < 0) { return -1; }

	read(fd, &status, 1);
	close(fd);
	printf("Rvex_core_status is %d", status);

	return 0;
}
