// CS3413 - Operating Systems I
// Assignment 1 - Question 2
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

int main(int argc, char *argv[]) {
	
	// File I/O variables.
	file = fopen("data.txt", "r");
	char line[1024];
	memset(line, 0, 1024);
	
	// Variables used throughout the function.
	char tProcess;
	int tArrival;
	int tDuration;
	struct Person info[10];
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

	int curTime = front->arrival;
	
	printf("Time	Job\n");

	int k = 0;
	int timeDone[n];

	while(k < count) {
		for(i = 0; i < n; i++) {
			if(curTime >= front->arrival) {
				if(curTime >= timeDone[i]) {
					printf("%d	%c\n", curTime, front->process);
					timeDone[i] = curTime + front->duration;
					front = front->next;
					k++;
				}
			}
		}

		curTime++;
		//printf("%d\n", time);
		//printf("1:%d	2:%d\n", timeDone[0], timeDone[1]);
	}
		
	/*// This for loop prints the time and the job executing at that time.
	for(i = 0; i < count; i++) {
		printf("%d	%c\n", time, front->process);
		time += front->duration;
		
		// Updates the last time a process executed for a specific user.
		for(j = 0; j < count; j++) {
			if(strcmp(info[j].name, front->user) == 0) {
				info[j].last = time;
				break;
			}
		}

		front = front->next;
	}

	printf("%d	IDLE\n\nSummary\n", time); 

	// Prints the summary.
	for(i = 0; i < (count - dup); i++) {
		printf("%s	%d\n", info[i].name, info[i].last);
	}
	*/
	return 0;
}
