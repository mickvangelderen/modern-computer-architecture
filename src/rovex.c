#include <stdio.h>
#include "rovex.h"

const size_t BUFFER_SIZE = 1024;
char buffer[1024];

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
