// CS3413 - Operating Systems I
// Lab 3 - Semaphores
// Joel Stanley
// Due October 22, 2015

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

// Declare semaphores.
sem_t agentSem;
sem_t semA, semB, semC;

void* smokerA() {	

	for( ; ; ) {
		sem_wait(&semA);
		printf("Smoker with tobacco will start smoking.\n");
		sleep(rand() % 10 + 1); // Sleeps for a random time between 1 and 10.
		printf("Smoker with tobacco has finished smoking.\n");
		sem_post(&agentSem);
	}
}

void* smokerB() {

	for( ; ; ) {
		sem_wait(&semB);
		printf("Smoker with paper will start smoking.\n");
		sleep(rand() % 10 + 1); // Sleeps for a random time between 1 and 10.
		printf("Smoker with paper has finished smoking.\n");
		sem_post(&agentSem);
	}
}

void* smokerC() {
	
	for ( ; ; ) {
		sem_wait(&semC);
		printf("Smoker with matches will start smoking.\n");
		sleep(rand() % 10 + 1); // Sleeps for a random time between 1 and 10.
		printf("Smoker with matches has finished smoking.\n");
		sem_post(&agentSem);
	}
}

void* agentFunc() {

	for ( ; ; ) {
		sem_wait(&agentSem);		
		int items = rand() % 3;

		switch(items) {
			case 0 : sem_post(&semA); // Items are paper and matches.
			printf("\nItems placed are paper and matches.\n");
			break;
			case 1 : sem_post(&semB); // Items are tobacco and matches.
			printf("\nItems placed are tobacco and matches.\n");
			break;
			case 2 : sem_post(&semC); // Items are tobacco and paper.
			printf("\nItems placed are tobacco and paper.\n");
			break;
		}
	}
}

int main() {
	
	// Initialize the semaphores/
	sem_init(&agentSem, 0, 1);
	sem_init(&semA, 0, 0);
	sem_init(&semB, 0, 0);
	sem_init(&semC, 0 ,0);	

	// Declares the pthreads.
	pthread_t a, b, c;
	pthread_t agent;

	// Creates the 3 smoker threads.
	pthread_create(&a, NULL, &smokerA, NULL);
	pthread_create(&b, NULL, &smokerB, NULL);
	pthread_create(&c, NULL, &smokerC, NULL);

	// Creates and joins the agent thread.
	pthread_create(&agent, NULL, &agentFunc, NULL);
	pthread_join(agent, NULL);
}
