#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

const size_t BUFFER_SIZE = 1024;
char buffer[1024];

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

int main(int argc, char **argv) {

	if (argc != 3) {
		printf("Use like this: `%s if of` where if is the path to a binary file and of will contain the output\n", argv[0]);
		return -1;
	}

	const char * inputFilePath = argv[1];
	const char * outputFilePath = argv[2];

	FILE * inputFile = myfopen(inputFilePath, "rb");
	FILE * outputFile = myfopen(outputFilePath, "wb");

	size_t numBytesRead;

	char hexBuffer[6];

	int i;

	if (inputFile == NULL || outputFile == NULL) {
		return -1;
	}

	sprintf(buffer, "char bytecode[] = {");
	fwrite(buffer, sizeof(char), strlen(buffer), outputFile);
	while ((numBytesRead = fread(buffer, sizeof(char), BUFFER_SIZE, inputFile)) != 0) {
		printf("Read %d bytes...\n", (int) numBytesRead);
		for (i = 0; i < numBytesRead; i++) {
			if ((i % 16) == 0) {
				sprintf(hexBuffer, "\n\t");
				fwrite(hexBuffer, sizeof(char), 2, outputFile);
			}
			sprintf(hexBuffer, "0x%02X,", buffer[i] & 0xFF);
			fwrite(hexBuffer, sizeof(char), 5, outputFile);
		}
	}
	fseek(outputFile, -1, SEEK_CUR);
	sprintf(buffer, "\n};");
	fwrite(buffer, sizeof(char), strlen(buffer), outputFile);

	fclose(inputFile);
	fclose(outputFile);
	return 0;
}
