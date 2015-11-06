// CS3413 - Operating Systems I
// Lab 4 - Memory Allocation
// Joel Stanley
// Due November 5, 2015

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {	

	for( ; ; ) {
		char *buffer = (char*)malloc(1024)
		printf("Memory Allocated\n");
		sleep(5);
	}

	return 0;
}

// When this program is run, it will infinitly call malloc and alocate memory to buffer. When using the free command in another console window, the free amount of memory will be reduced by 1K for
// each call of malloc.
