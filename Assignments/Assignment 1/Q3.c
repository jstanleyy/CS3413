// CS3413 - Operating Systems I
// Assignment 1 - Question 3
// Joel Stanley
// Due October 1, 2015

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Used in the linked list queue.
struct Node {
	char *user;
	char process;
	int arrival;
	int duration;
	struct Node *next;
};

// Used to keep track of the last job each person finished.
struct Person {
	char *name;
	int last;
};

//Pointers for Queue.
struct Node* front = NULL;
struct Node* end = NULL;

FILE *file;

// Adds the Node structure to the end of the queue.
void enqueue(char *userIn, char processIn, int arrivalIn, int durationIn) {
	struct Node* new = (struct Node*)malloc(sizeof(struct Node));
	new->user = userIn;
	new->process = processIn;
	new->arrival = arrivalIn;
	new->duration = durationIn;
	new->next = NULL;

	if(front == NULL && end == NULL) {
		front = end = new;
	}
	else {
		end->next = new;
		end = new;
	}
} 

struct Person info[10];

int main(int argc, char *argv[]) {
	
	// File I/O variables.
	file = fopen("data.txt", "r");
	char line[1024];
	memset(line, 0, 1024);
	
	// Variables used throughout the function.
	char tProcess;
	int tArrival;
	int tDuration;
	int count = 0, done = 0;
	int i, skip = 0, j, dup = 0;
	int n = *argv[1] - 48;

	fgets(line, 1024, file); // Skips the first line of the input.
	
	while(done == 0) {
		char *tUser = (char *)malloc(sizeof(char[20])); // Allocates memory to be used for the input names
		if(fscanf(file,"%s	%c	%d	%d", tUser, &tProcess, &tArrival, &tDuration) == 4) {
			enqueue(tUser, tProcess, tArrival, tDuration);
		
			// Checks if the name already exists in the array, and skips if so.
			for(i = 0; i < count; i++) {
				if(strcmp(info[i].name, tUser) == 0) { 
					skip = 1;
					dup++;
				}
			}

			// Adds the person to the array if not already there.
			if(skip == 0) {
				struct Person person;
				person.name = tUser;
				info[count] = person;
			}

			count = count + 1; // Keeps track of how many lines of input have been read.
		}
		else {
			done = 1;
		}
	}
	
	fclose(file); // Closes the file as it is no longer needed.

	int time = front->arrival;

	printf("Time	Job\n");

	/*int k = 0;
	int timeDone[n]; // Used to keep track of cores.

	// Makes sure every element is set to 0
	for(i = 0; i < n; i++) {
		timeDone[i] = 0;
	} 
	*/
	// Prints each process that is executing on each CPU.
	while(k < count) {
		for(i = 0; i < n; i++) { // Loops for each CPU.
			if(time >= front->arrival) { // Can only proceed if the next task has arrived.
				if(time >= timeDone[i]) { // Is a CPU not being used?
					printf("%d	%c\n", time, front->process);
					timeDone[i] = time + front->duration; // Updates when that CPU will be free.
				
					// Keeps track of the last job for each user.
					for(j = 0; j < count; j++) {
						if(strcmp(info[j].name, front->user) == 0) {
							info[j].last = timeDone[i];
							break;
						}
					}
					
					front = front->next;
					k++;
				}
			}

			if (k >= count) {
				break;
			}
		}
	
		time++;
	}
		
	// Prints the Idle times for each CPU.
	for(i = 0; i < n; i++) {
		printf("%d	CPU %d IDLE\n", timeDone[i], i + 1);
	}
	
	// Prints the Summary.
	printf("\nSummary\n");

	for(i = 0; i < (count-dup); i++) {
		printf("%s	%d\n", info[i].name, info[i].last);
	}	

	return 0;
}
