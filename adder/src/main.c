
#include <stdio.h>

int main(int argc, char **argv) {

	unsigned char RVEX_CLEAR = 2;
	unsigned char RVEX_START = 1;

	FILE* f;
	unsigned char status = 0;

	f = fopen("/sys/devices/virtual/rvex_core/rvex-smemory.0/rvex_core_ctl", "w");
	if (f == NULL || ferror(f)) {
		perror("Error while opening /sys/devices/virtual/rvex_core/rvex-smemory.0/rvex_core_ctl");
		return -1;
	} else {
		fwrite(&RVEX_CLEAR, sizeof(unsigned char), 1, f);
		fclose(f);
	}
	
	printf("Wrote clear to rvex_core_ctl");

	f = fopen("/sys/devices/virtual/rvex_core/rvex-smemory.0/rvex_core_status", "r");
	if (f == NULL || ferror(f)) {
		perror("Error while opening /sys/devices/virtual/rvex_core/rvex-smemory.0/rvex_core_status");
		return -1;
	} else {
		fread(&status, sizeof(unsigned char), 1, f);
		fclose(f);
	}

	printf("Rvex_core_status is %d", status);

	return 0;
}
