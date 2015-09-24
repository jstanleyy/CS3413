// CS3413 - Operating Systems I
// Lab 1 - Processes
// Joel Stanley
// Due September 24, 2015

/*
	2. The Child ID is always the Parent ID + 1. We get the values of var because the child process
 	has a seperate heap from the parent.

	4. The clone system call is responsible for the creation of the child process.
*/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void * run(void *in) {
	printf("The thread ID is: %d.\n", pthread_self());
}

int main() {
	int var = 10; // Test variable.
	pid_t process = fork(); // Forks to create a child process.

	// Checks if there was an error creating the process, and stops if so.
	if(process < 0) {
		printf("Error creating process.");
		return -1; // Program encountered an error.
	}

	// Will only runs in the child process.
	else if (process == 0) {
		printf("Child Process ID is %d.\n", getpid());
		var += 5;
		printf("%d\n", var);
		pthread_t child; //Defines the pthread to be created.
		pthread_create(&child, NULL, &run, NULL); // Creates the pthread to execute the run function.
		pthread_join(child, NULL); // Joins the pthread.
	}

	// Will only run in the parent process.
	else {
		printf("Parent Process ID is %d.\n", getpid());
		var -= 5;
        printf("%d\n", var);
	}
	
	return 0; // Program ran successfully.
}
