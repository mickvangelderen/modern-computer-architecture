
#define DATASIZE 5

int x[DATASIZE] = {1, 2, 3, 4, 5};
int c = 0;

int main() {
	int i = DATASIZE;
	while (i--) {
		c += x[i];
	}
	return 0;
}

