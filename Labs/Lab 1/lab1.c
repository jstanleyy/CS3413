// CS3413 - Operating Systems I
// Lab 1
// Joel Stanley
// Due September 24th 2015

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main () {
	pid_t pid = fork();
	
	if(pid == 0) {
		
		printf("Child Process ID is %d", getpid());
	}
	return 0;
}