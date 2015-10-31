// CS3415 - Operating Systems I
// Assignment 3 - Question 2
// Joel Stanley
// Due November 2, 2015

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#define THINKING 0
#define HUNGRY 1
#define EATING 2

// Global variables
pthread_mutex_t mutex[5];
pthread_cond_t condition[5];
int state[5];

void pickup_forks(int id) {

	pthread_mutex_lock(&mutex[id]);
	state[id] = HUNGRY;

	while(state[(id - 1) % 5] == EATING || state[(id + 1) % 5] == EATING) {
		pthread_cond_wait(&condition[id], &mutex[id]);
	}

	state[id] = EATING;
	pthread_mutex_unlock(&mutex[id]);
}

void return_forks(int id) {
	pthread_mutex_lock(&mutex[id]);
	state[id] = THINKING;
	if(state[(id - 1) % 5] == HUNGRY) {
		pthread_cond_signal(&condition[(id - 1) % 5]);
	}
	if(state[(id + 1) % 5] == HUNGRY) {
		pthread_cond_signal(&condition[(id + 1) % 5]);
	}
	pthread_mutex_unlock(&mutex[id]);
}
void* run (void *in) {
	int id = *((int *) in);

	for( ; ; ) {
		state[id] = THINKING;
		printf("Philosopher %d is thinking.\n", id);
		sleep(rand() % 3 + 1); // Thinking for 1 to 3 seconds.
		pickup_forks(id);
		printf("Philosopher %d is eating.\n", id);
		sleep(rand() % 3 + 1); // Eating for 1 to 3 seconds.
		return_forks(id);
	}

}

int main() {

	int i;

	for(i = 0; i < 5; i++) {	
		pthread_mutex_init(&mutex[i], NULL);
		pthread_cond_init(&condition[i], NULL);
	}

	pthread_t philosophers[5];
	for(i = 0; i < 5; i++) {
		int *arg = malloc(sizeof(*arg));
		*arg = i;
		pthread_create(&philosophers[i], NULL, &run, arg); 
	}

	for(i = 0; i < 5; i++) {
		pthread_join(philosophers[i], NULL);
	}

	return 0;
}
