// CS3413 - Operating Systems I
// Lab 2 - Pipes
// Joel Stanley
// Due October 8, 2015

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <limits.h>

#define READ_END 0
#define WRITE_END 1

// The global variables used to hold the outputs.
int average = 0; 
int min = INT_MAX; 
int max = 0;

// Function declarations.
void * getAverage(void *in);
void * getMin(void *in);
void * getMax(void *in);

// Declares the file descriptors.
int fd[2]; 
int fd2[2];
int fd3[2];

int main() {

	// Creates the 3 pipes for each thread.
	if (pipe(fd) == -1) {
		printf("Pipe 1 failed.");
		return 1;
	}

	if (pipe(fd2) == -1) {
		printf("Pipe 2 failed.");
		return 1;
	}

	if (pipe(fd3) == -1) {
		printf("Pipe 3 failed.");
		return 1;
	}

	// Declares and creates a thread for each task.
	pthread_t pAverage, pMin, pMax;
	pthread_create(&pAverage, NULL, &getAverage, NULL);
	pthread_create(&pMin, NULL, &getMin, NULL);
	pthread_create(&pMax, NULL, &getMax, NULL);
	
	int input, done = 0;

	// Gets input and sends the numbers to each thread.
	while (done == 0) {
		printf("Input a number (0 to finish): ");
		scanf("%d", &input);
		
		if (input == 0) {
			done = 1;
		}
		else {
			write(fd[WRITE_END], &input, sizeof(input));
			write(fd2[WRITE_END], &input, sizeof(input));
			write(fd3[WRITE_END], &input, sizeof(input));
		}
	}

	// Prints the solutions from each thread
	printf("The average value is %d.\n", average);
	printf("The minimum value is %d.\n", min);
	printf("The maximum value is %d.\n", max);

	return 0;
}

// Reads in from the pipe fd, and calculates the maximum value.
void * getMax(void *in) {
	int read_max;

	for( ; ; ) {
		read(fd[READ_END], &read_max, sizeof(int));
		if (read_max > max) {
			max = read_max;
		}
	}
}

// Reads in from the pipe fd2, and calculates the minimum value.
void * getMin(void *in) {
	int read_min;

	for( ; ; ) {
		read(fd2[READ_END], &read_min, sizeof(int));
		if (read_min < min) {
			min = read_min;
		}
	}
}

// Reads in from the pipe fd3, and calculates the average.
void * getAverage(void *in) {
	int read_avg;
	int sum = 0, count = 0;

	for( ; ; ){
	    read(fd3[READ_END], &read_avg, sizeof(int));
		sum = sum + read_avg;
		count = count + 1;
		//printf("sum: %d, count: %d\n", sum, count);
		average = sum / count;
		//printf("average: %d\n", average);
	}
}
