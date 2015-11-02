// CS3415 - Operating Systems I
// Assignment 3 - Question 3
// Joel Stanley
// Due November 2, 2015

#include <stdio.h>
#include <stdlib>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

struct Node {
	char *name;
	int direction;
	int arrival;
	struct Node *next;
};

// Pointers for each queue.
struct Node* frontN = NULL;
struct Node* endN = NULL;
struct Node* frontS = NULL;
struct Node* endS = NULL;

// Global Variables
FILE *file;
int timeToCross;

void enqueue(char *nameIn, int directIn, int arrivalIn) {

	struct Node* new = (struct Node*)malloc(sizeof(struct Node));
	new->name = nameIn;
	new->direction = directIn;
	new->arrival = arrivalIn;
	new->next = NULL;
	
	if(new->direction == 0) {
		if(frontN == NULL && endN == NULL) {
			frontN = endN = new;
		}
		else {
			endN->next = new;
			endN = new;
		}
	}
	else {
		if(frontS == NULL && endS == NULL) {
			frontS = endS = new;
		}
		else {
			endS->next = new;
			endS = new;
		}
	}
}

int main(int argc, char *argv[]) {

	// File I/O variables	
	file = fopen("data.txt", "r");
	char line[1024];
	memset(line, 0, 1024);

	int done = 0;
	int direction, arrival;
	timeToCross = atoi(argv[1]);

	fgets(line, 1024, file); // Skips the first line of the input.

	while(done == 0) {
		char *name = (char *)malloc(sizeof(char[20]));
		if(fscanf(file, "%s	%d	%d", name, &direction, &arrival) == 3) {
			enqueue(name, direction, arrival);
		else {
			done = 1;
		}
	}

	int time = 0;
	done = 0;
	int curDirection, carsOnBridge = 0;
	
	if(frontN->arrival <= frontS->arrival) {
		curDirection = 0;
		time = frontN->arrival;
	}
	else {
		curDirection = 1;
		time = frontS->arrival;
	}

	while(done == 0) {

		if(frontN == NULL) {
			curDirection = 1;
		}
		else if(frontS == NULL) {
			curDirection = 0;
		}
		
		if(time >= frontN->arrival && curDirection == 0 && carsOnBridge < 3 ) {
			// Take from frontN
		}
		else if(time >= frontS->arrival && curDirection == 1 && carsOnBridge < 3)  {
			// Take from frontS
		}
	}

	return 0;
}
