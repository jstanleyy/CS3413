// CS3413 - Operating Systems I
// Lab 4 - Memory Allocation
// Joel Stanley
// Due November 5, 2015

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {	

	int i;
	char* current;

	for(i = 0; i < 100000; i++) {
		current = (char*)malloc(1024);
		printf("%d\n", current);
		sleep(2);
	}
	return 0;
}

// By looking at the addresses of each call to malloc, we can see that the average difference is 1040. This is because it allocates the 1024 thats asked, then also allocates 16bits for book keeping
// info like how much memory was allocated for that malloc call.
