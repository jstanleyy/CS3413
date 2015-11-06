// CS3413 - Operating Systems I
// Lab 4 - Memory Allocation
// Joel Stanley
// Due November 5, 2015

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {	

	int i;
	char* current[5];

	for(i = 0; i < 5; i++) {
		current[i] = (char*)malloc(1024);
		printf("%d\n", current[i]);
		sleep(2);
	}

	printf("\n");
	free(current[2]);
	free(current[4]);

	for(i = 0; i < 5; i++) {
		current[i] = (char*)malloc(1024);
		printf("%d\n", current[i]);
		sleep(2);
	}

	return 0;
}

//Yes, when running this program, you will see that the addresses freed from the first loop are then resued in the second loop.
