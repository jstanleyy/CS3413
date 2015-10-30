// CS3413 - Operating Systems I
// Assignment 3 - Question 1
// Joel Stanley
// Due November 2, 2015

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

// Global variables
int numOfChairs;
sem_t barbReady, custReady, seats, done;


void* barber() {

	for( ; ; ) { // Infinite loop
		sem_wait(&custReady); // Waits for a customer.
		sem_wait(&seats); // Waits for access to change the number of available seats.
		numOfChairs += 1; // A chair becomes available.
		//sem_post(&barbReady);
		sem_post(&seats);
		sleep(10);
		printf("I cut a customers hair.\n");
		sem_post(&barbReady);
	}
}

void* customer() {

	printf("A new customer arrived.\n");
	wait(&seats);
	if(numOfChairs > 0) {
		numOfChairs -= 1;
		sem_post(&custReady);
		sem_post(&seats);
		sem_wait(&barbReady);
		printf("I had my hair cut.\n");
	}
	else {
		sem_post(&seats);
		printf("No seats, I'm leaving.\n");
	}
}

int main(int argc, char *argv[]) {

	numOfChairs	= atoi(argv[1]);
	printf("Number of chairs is: %d\n", numOfChairs);

	sem_init(&barbReady, 0, 0);
	sem_init(&custReady, 0, 0);
	sem_init(&seats, 0, 1);
	sem_init(&done, 0, 0);
	
	pthread_t barbThread;

	pthread_create(&barbThread, NULL, &barber, NULL);
	printf("Thread created.\n");

	for ( ; ; ) {
		customer();
		sleep(2);
	}
}
