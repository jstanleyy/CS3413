// CS3413 - Operating Systems I
// Lab 4 - Memory Allocation
// Joel Stanley
// Due November 5, 2015

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {	

	for( ; ; ) {
		char *buffer = (char*)malloc(9999999999);
	}

	return 0;
}
